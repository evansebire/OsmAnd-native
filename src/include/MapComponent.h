#include "OsmAndConfig.h"
#include <QTime>
#include <QtQuick/QQuickItem>
#include <OsmAndCore/Map/IMapRenderer.h>

class QOpenGLContext;

class MapComponent : public QQuickItem {
  Q_OBJECT

  bool _initialized;
  OsmAndCfgMap *_config;
  std::shared_ptr<OsmAnd::IMapRenderer> _renderer;
  QOpenGLContext *_context;
  
  double _dotsPerCm;
  int _lastX;
  int _lastY;

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
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  
signals:
  void scaleChanged();
  
public slots:
  void paint();
  
  void updateCfg();
  void zoomIn();
  void zoomOut();

private slots:
  void handleWindowChanged(QQuickWindow *win);  
};
