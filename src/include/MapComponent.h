#include "OsmAndConfig.h"
#include <QTime>
#include <QtQuick/QQuickItem>
#include <OsmAndCore/Map/IMapRenderer.h>

#ifdef OSMAND_OPENGLES2_RENDERER_SUPPORTED
#include "EGL/egl.h"
#endif


class QOpenGLContext;
class GestureRecognizer;

class MapComponent : public QQuickItem {
  Q_OBJECT

  bool _initialized;
  OsmAndCfgMap *_config;
  std::shared_ptr<OsmAnd::IMapRenderer> _renderer;

#if defined(OSMAND_OPENGL_RENDERER_SUPPORTED)
  QOpenGLContext *_context;
#elif defined(OSMAND_OPENGLES2_RENDERER_SUPPORTED)
  EGLContext _context;
#endif

  GestureRecognizer *_recognizer;
  double _dotsPerCm;

signals:
  void distanceToFirstCPChanged(double);
  void distanceChanged(double);
  void speedChanged(double);
  void altitudeChanged(double);
  void timeToDestinationChanged(QTime);
  
public:
  Q_PROPERTY(double scale READ getScale NOTIFY scaleChanged);
  Q_PROPERTY(OsmAndCfgMap* config READ getConfig WRITE setConfig);
  Q_PROPERTY_AUTO_NOTIFY(double, distanceToFirstCP);
  Q_PROPERTY_AUTO_NOTIFY(double, distance);
  Q_PROPERTY_AUTO_NOTIFY(double, speed);
  Q_PROPERTY_AUTO_NOTIFY(double, altitude);
  Q_PROPERTY_AUTO_NOTIFY(QTime, timeToDestination);
  
public:
  MapComponent();
  OsmAndCfgMap *getConfig() { return _config; }
  void setConfig(OsmAndCfgMap *config);
  
  double getScale();

protected:
  void touchEvent(QTouchEvent *event);  
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  
signals:
  void scaleChanged();
  
public slots:
  void pinch(int deltaX, int deltaY, double scale, double angleChanged);
  void drag3f(int delta);  
  void paint();
  void resized();
  
  void updateCfg();
  void zoomIn();
  void zoomOut();

private slots:
  void handleWindowChanged(QQuickWindow *win);  
};
