#include "MapComponent.h"
#include <QOpenGLContext>
#include <QScreen>
#include <QtMath>
#include <QtQuick/QQuickWindow>

#include <OsmAndCore/Map/OnlineMapRasterTileProvider.h>
#include <OsmAndCore/Utilities.h>
#include "GestureRecognizer.h"


MapComponent::MapComponent() : _initialized(false),
  _config(0),
  distanceToFirstCP(0.0),
  distance(0.0),
  speed(0.0),
  altitude(0.0),
  timeToDestination(QTime(0, 0))
{
  setAcceptedMouseButtons(Qt::LeftButton);
  
#if defined(OSMAND_OPENGL_RENDERER_SUPPORTED)
  _renderer = OsmAnd::createAtlasMapRenderer_OpenGL();
#elif defined(OSMAND_OPENGLES2_RENDERER_SUPPORTED)
  _renderer = OsmAnd::createAtlasMapRenderer_OpenGLES2();
#else
#error "No renderer available"
#endif

  QDir dataDirectory(OsmAndCfgMap::appRootDirectory());
  auto tileProvider = OsmAnd::OnlineMapRasterTileProvider::createCycleMapProvider();
  static_cast<OsmAnd::OnlineMapRasterTileProvider*>(tileProvider.get())->setLocalCachePath(dataDirectory);
  _renderer->setRasterLayerProvider(OsmAnd::RasterMapLayerId::BaseLayer, tileProvider);
  _renderer->setRasterLayerOpacity(OsmAnd::RasterMapLayerId::BaseLayer, 0.5f);
  _renderer->setFogColor(OsmAnd::FColorRGB(1.0f, 1.0f, 1.0f));

  _recognizer = new GestureRecognizer;  
  connect(this, SIGNAL(windowChanged(QQuickWindow*)),
          this, SLOT(handleWindowChanged(QQuickWindow*)));
  connect(_recognizer, SIGNAL(pinch(int,int,double,double)),
          this, SLOT(pinch(int,int,double,double)));
  connect(_recognizer, SIGNAL(drag3f(int)),
          this, SLOT(drag3f(int)));  
}


void MapComponent::setConfig(OsmAndCfgMap *config)
{
  _config = config;
  updateCfg();
  connect(_config, SIGNAL(updated()), this, SLOT(updateCfg()));
}


void MapComponent::handleWindowChanged(QQuickWindow *win)
{
  if (win) {
    _dotsPerCm = win->screen()->logicalDotsPerInch() / 2.54;
    
    connect(win, SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
    win->setClearBeforeRendering(false);
    
    OsmAnd::MapRendererSetupOptions options;
    options.backgroundWorker.enabled = false;
    options.frameRequestCallback = [win]() { QMetaObject::invokeMethod(win, "update"); };
    _renderer->setup(options);
  }
}


void MapComponent::paint()
{
#if defined(OSMAND_OPENGL_RENDERER_SUPPORTED)
  QOpenGLContext *savedContext = QOpenGLContext::currentContext();
  
  if (!_context) {
    _context = new QOpenGLContext;
    QSurfaceFormat fmt = window()->format();
    fmt.setMajorVersion(3);
    fmt.setMinorVersion(0);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    _context->setFormat(fmt);
    _context->create();
  }
  
  _context->makeCurrent(window());
#endif

  if (!_initialized)
    _initialized = _renderer->initializeRendering();
  
  OsmAnd::AreaI viewport;
  viewport.top = y();
  viewport.left = x();
  viewport.bottom = height()-y();
  viewport.right = width()-x(); 
  _renderer->setWindowSize(OsmAnd::PointI(width(), height()));
  _renderer->setViewport(viewport);  

  if(_renderer->prepareFrame())
    _renderer->renderFrame();
  _renderer->processRendering();

#if defined(OSMAND_OPENGL_RENDERER_SUPPORTED)
  savedContext->makeCurrent(window());
#elif defined(OSMAND_OPENGLES2_RENDERER_SUPPORTED)
  glActiveTexture(GL_TEXTURE0);
#endif
}


void MapComponent::pinch(int deltaX, int deltaY, double scale, double angleChanged)
{
  float angle = qDegreesToRadians(_renderer->state.azimuth);
  float cosAngle = cosf(angle);
  float sinAngle = sinf(angle);
  
  float nx = (-deltaX) * cosAngle - (-deltaY) * sinAngle;
  float ny = (-deltaX) * sinAngle + (-deltaY) * cosAngle;
  
  int32_t tileSize31 = 1;
  if(_renderer->state.zoomBase != 31)
    tileSize31 = (1u << (31 - _renderer->state.zoomBase)) - 1;
  float scale31 = (float)tileSize31 / _renderer->getScaledTileSizeOnScreen();
  
  OsmAnd::PointI newPosition;
  OsmAnd::PointI oldPosition = _renderer->state.target31;
  newPosition.x = oldPosition.x + nx * scale31;
  newPosition.y = oldPosition.y + ny * scale31;

  _renderer->setTarget(newPosition);
  _renderer->setZoom(_renderer->state.requestedZoom + scale - 1.0);
  _renderer->setAzimuth(_renderer->state.azimuth - qRadiansToDegrees(angleChanged), true);
  if (_config) {
    _config->X = _renderer->state.target31.x;
    _config->Y = _renderer->state.target31.y;
    _config->Zoom = _renderer->state.requestedZoom;
    _config->Azimuth = _renderer->state.azimuth;
  }
  
  emit scaleChanged();
}

void MapComponent::drag3f(int delta)
{
  double newAngle = _renderer->state.elevationAngle + 90.0 * (double)delta / (window()->height() / 2);
  _renderer->setElevationAngle(newAngle, true);
  if (_config) {
    _config->ElevationAngle = _renderer->state.elevationAngle;
  }
}

void MapComponent::touchEvent(QTouchEvent *event)
{
  if (!_recognizer->process(event))
    QQuickItem::touchEvent(event);
}

void MapComponent::mouseMoveEvent(QMouseEvent *event)
{
  if (!_recognizer->process(event))
    QQuickItem::mouseMoveEvent(event);
}


void MapComponent::mousePressEvent(QMouseEvent *event)
{
  if (!_recognizer->process(event))
    QQuickItem::mousePressEvent(event);
}

void MapComponent::wheelEvent(QWheelEvent *event)
{
  if (!_recognizer->process(event))
    QQuickItem::wheelEvent(event);
}


double MapComponent::getScale()
{
  const static double C = 40075017.0;  
  double latitude = OsmAnd::Utilities::get31LatitudeY(_renderer->state.target31.y);
  double S = C*cos(OsmAnd::Utilities::toRadians(latitude))/qPow(2, (_renderer->state.requestedZoom+8));
  return S * _dotsPerCm;
}


void MapComponent::updateCfg()
{
  _renderer->setTarget(OsmAnd::PointI(_config->X, _config->Y));
  _renderer->setAzimuth(_config->Azimuth);
  _renderer->setElevationAngle(_config->ElevationAngle);
  _renderer->setZoom(_config->Zoom, true);
  emit scaleChanged();
}


void MapComponent::zoomIn()
{
  _renderer->setZoom(_renderer->state.requestedZoom + 1.0f, true);
  if (_config)
    _config->Zoom = _renderer->state.requestedZoom;
  emit scaleChanged();
}

void MapComponent::zoomOut()
{
  _renderer->setZoom(_renderer->state.requestedZoom - 1.0f, true);
  if (_config)
    _config->Zoom = _renderer->state.requestedZoom;
  emit scaleChanged();
}
