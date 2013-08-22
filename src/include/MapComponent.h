#include <QTime>
#include <QtQuick/QQuickItem>
#include <OsmAndCore/Map/IMapRenderer.h>

class QOpenGLContext;

class MapComponent : public QQuickItem {
  Q_OBJECT
  
private:
  bool _initialized;
  std::shared_ptr<OsmAnd::IMapRenderer> _renderer;
  QOpenGLContext *_context;
  
  double _dotsPerCm;
  int _lastX;
  int _lastY;
  
  double _distanceToFirstCP;
  double _distance;
  double _speed;
  double _altitude;
  QTime _timeToDestination;
  
  Q_PROPERTY(double scale READ getScale);
  
  Q_PROPERTY(double distanceToFirstCP READ distanceToFirstCP WRITE updateDistanceToFirstCP);
  Q_PROPERTY(double distance READ distance WRITE updateDistance);
  Q_PROPERTY(double speed READ speed WRITE updateSpeed);
  Q_PROPERTY(double altitude READ altitude WRITE updateAltitude);
  Q_PROPERTY(QTime timeToDestination READ timeToDestination WRITE updateTimeToDestination);
  
public:
  MapComponent();
  double distanceToFirstCP() { return _distanceToFirstCP; }
  double distance() { return _distance; }
  double speed() { return _speed; }
  double altitude() { return _altitude; }
  QTime timeToDestination() { return _timeToDestination; }
  
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
  
  void updateDistanceToFirstCP(double newValue) { _distanceToFirstCP = newValue; }  
  void updateDistance(double newValue) { _distance = newValue; }
  void updateSpeed(double newValue) { _speed = newValue; }
  void updateAltitude(double newValue) { _altitude = newValue; }
  void updateTimeToDestination(QTime newValue) { _timeToDestination = newValue; }

private slots:
  void handleWindowChanged(QQuickWindow *win);  
};
