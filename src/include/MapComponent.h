#include "OsmAndConfig.h"
#include <QTime>
#include <QtQuick/QQuickItem>
#include <OsmAndCore/Map/IMapRenderer.h>

class QOpenGLContext;

class MapComponent : public QQuickItem {
  Q_OBJECT

  OsmAndConfig *_config;
  bool _initialized;
  std::shared_ptr<OsmAnd::IMapRenderer> _renderer;
  QOpenGLContext *_context;
  
  double _dotsPerCm;
  int _lastX;
  int _lastY;

  Q_PROPERTY(double scale READ getScale);
  
  Q_PROPERTY_AUTO(double, distanceToFirstCP);
  Q_PROPERTY_AUTO(double, distance);
  Q_PROPERTY_AUTO(double, speed);
  Q_PROPERTY_AUTO(double, altitude);
  Q_PROPERTY_AUTO(QTime, timeToDestination);
  
public:
  MapComponent();
  void setConfig(OsmAndConfig *config);
  
  double getScale();

protected:
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  
signals:
  void scaleChanged();
  
public slots:
  void paint();
  void sync();
  void zoomIn();
  void zoomOut();

private slots:
  void handleWindowChanged(QQuickWindow *win);  
};
