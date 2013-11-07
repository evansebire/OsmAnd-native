#ifndef TOUCHAREA_H
#define TOUCHAREA_H

#include <QtQuick/QQuickItem>

class GestureRecognizer : public QObject {
  Q_OBJECT

private:
  enum {
    MaxPointsNum = 3
  };

  int _pointsCount;
  QPoint _mousePoint;
  QPointF _points[MaxPointsNum];
  QPointF _oldCenter;
  double _oldDistance;
  double _oldAngle;

  void grabPoints(QList<QTouchEvent::TouchPoint> &newPoints);
  bool checkPoints(QList<QTouchEvent::TouchPoint> &newPoints);

  double distance(QPointF point1, QPointF point2);
  double angle(QPointF point1, QPointF point2, double ptDistance);

public:
  GestureRecognizer() : _pointsCount(0), _oldDistance(0.0) {}
  bool process(QTouchEvent *event);
  bool process(QMouseEvent *event);
  bool process(QWheelEvent *event);

signals:
  void pinch(int xDelta, int yDelta, double scaleChanged, double angleChanged);
  void drag3f(int delta);
};

#endif // TOUCHAREA_H
