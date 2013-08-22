#include "MapComponent.h"
#include <QOpenGLContext>
#include <QScreen>
#include <QtMath>
#include <QtQuick/QQuickWindow>

#include <OsmAndCore/Map/OnlineMapRasterTileProvider.h>
#include <OsmAndCore/Utilities.h>

static QOpenGLContext *createContext()
{
  QOpenGLContext *_context = new QOpenGLContext;
  QSurfaceFormat fmt;
  fmt.setMajorVersion(3);
  fmt.setMinorVersion(0);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  _context->setFormat(fmt);
  return _context;
}

MapComponent::MapComponent() : _initialized(false), _lastX(0), _lastY(0),
  _distanceToFirstCP(0.0),
  _distance(0.0),
  _speed(0.0),
  _altitude(0.0),
  _timeToDestination(QTime(0, 0))
{
  setAcceptedMouseButtons(Qt::LeftButton);
  _renderer = OsmAnd::createAtlasMapRenderer_OpenGL();
  
  connect(this, SIGNAL(windowChanged(QQuickWindow*)),
          this, SLOT(handleWindowChanged(QQuickWindow*)));
}


void MapComponent::handleWindowChanged(QQuickWindow *win)
{
  if (win) {
    _dotsPerCm = win->screen()->logicalDotsPerInch() / 2.54;
    
    connect(win, SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
    connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
    win->setClearBeforeRendering(false);

    auto tileProvider = OsmAnd::OnlineMapRasterTileProvider::createCycleMapProvider();
    static_cast<OsmAnd::OnlineMapRasterTileProvider*>(tileProvider.get())->setLocalCachePath(QDir::current());
    _renderer->setRasterLayerProvider(OsmAnd::BaseLayer, tileProvider);
    _renderer->setRasterLayerOpacity(OsmAnd::BaseLayer, 0.5f);
    _renderer->setFogColor(OsmAnd::FColorRGB(1.0f, 1.0f, 1.0f));
    _renderer->setTarget(OsmAnd::PointI(OsmAnd::Utilities::get31TileNumberX(30.392386),
                                        OsmAnd::Utilities::get31TileNumberY(59.964222)));
    _renderer->setZoom(12.5f);
    _renderer->setAzimuth(0.0f);
    _renderer->setElevationAngle(90.0f);

    OsmAnd::MapRendererSetupOptions options;
    options.backgroundWorker.enabled = false;
    options.frameRequestCallback = [win]() { QMetaObject::invokeMethod(win, "update"); };
    _renderer->setup(options);
  }
}


void MapComponent::paint()
{
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
  if (!_initialized)
    _initialized = _renderer->initializeRendering();
  
  OsmAnd::AreaI viewport;
  viewport.top = y();
  viewport.left = x();
  viewport.bottom = height()-y();
  viewport.right = width()-x(); 
  _renderer->setWindowSize(OsmAnd::PointI(width(), height()));
  _renderer->setViewport(viewport);  

  _renderer->processRendering();
  _renderer->renderFrame();
  _renderer->postprocessRendering();
  
  savedContext->makeCurrent(window());
}


void MapComponent::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton) {
    int dx = _lastX - event->x();
    int dy = _lastY - event->y();

    float angle = qDegreesToRadians(_renderer->state.azimuth);
    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);
    
    float nx = dx * cosAngle - dy * sinAngle;
    float ny = dx * sinAngle + dy * cosAngle;
    
    int32_t tileSize31 = 1;
    if(_renderer->state.zoomBase != 31)
      tileSize31 = (1u << (31 - _renderer->state.zoomBase)) - 1;
    float scale31 = (float)tileSize31 / _renderer->getScaledTileSizeOnScreen();
    
    OsmAnd::PointI newPosition;
    OsmAnd::PointI oldPosition = _renderer->state.target31;    
    newPosition.x = oldPosition.x + nx * scale31;
    newPosition.y = oldPosition.y + ny * scale31;
    _renderer->setTarget(newPosition, true);
    
    _lastX = event->x();
    _lastY = event->y();
    emit scaleChanged();
  }
}


void MapComponent::mousePressEvent(QMouseEvent *event)
{
  _lastX = event->x();
  _lastY = event->y();
}

void MapComponent::wheelEvent(QWheelEvent *event)
{
  float step = event->delta() > 0 ? 0.1f : -0.1f;
  _renderer->setZoom(_renderer->state.requestedZoom + step, true);
  emit scaleChanged();
}

void MapComponent::sync()
{
}

double MapComponent::getScale()
{
  const static double C = 40075017.0;  
  double latitude = OsmAnd::Utilities::get31LatitudeY(_renderer->state.target31.y);
  double S = C*cos(OsmAnd::Utilities::toRadians(latitude))/qPow(2, (_renderer->state.requestedZoom+8));
  return S * _dotsPerCm;
}


void MapComponent::zoomIn()
{
  _renderer->setZoom(_renderer->state.requestedZoom + 1.0f, true);
  emit scaleChanged();
}

void MapComponent::zoomOut()
{
  _renderer->setZoom(_renderer->state.requestedZoom - 1.0f, true);
  emit scaleChanged();
}