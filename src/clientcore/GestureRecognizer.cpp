#include "GestureRecognizer.h"
#include <math.h>


bool GestureRecognizer::checkPoints(QList<QTouchEvent::TouchPoint> &newPoints)
{
  for (int i = 0; i < newPoints.size(); i++) {
    if (newPoints[i].state() & Qt::TouchPointReleased)
      return false;
  }

  return true;
}

double GestureRecognizer::distance(QPointF point1, QPointF point2)
{
  double xDiff = fabs(point1.x() - point2.x());
  double yDiff = fabs(point1.y() - point2.y());
  return sqrt(xDiff*xDiff + yDiff*yDiff);
}

double GestureRecognizer::angle(QPointF point1, QPointF point2, double ptDistance)
{
  double alpha = asin((point2.y() - point1.y()) / ptDistance);
  if (point2.x() - point1.x() >= 0) {
    if (point2.y() > point1.y())
      return alpha;
    else
      return 2*M_PI + alpha;
  } else {
    return M_PI - alpha;
  }
}

bool GestureRecognizer::process(QTouchEvent *event)
{
  switch (event->type()) {
    case QEvent::TouchBegin :
    case QEvent::TouchUpdate :
    case QEvent::TouchEnd : {
      QList<QTouchEvent::TouchPoint> points = event->touchPoints();
      if (!checkPoints(points)) {
        _pointsCount = 0;
        break;
      }

      if (points.size() != _pointsCount) {
        // change gesture type
        switch (points.size()) {
          case 1 :
            _points[0] = points[0].pos();
            break;
          case 2 :
            _oldCenter = (points[0].pos() + points[1].pos()) / 2;
            _oldDistance = distance(points[0].pos(), points[1].pos());
            _oldAngle = angle(points[0].pos(), points[1].pos(), _oldDistance);
            break;
          case 3 :
            _points[0] = points[0].pos();
            _points[1] = points[1].pos();
            _points[2] = points[2].pos();
            break;
        }
        _pointsCount = points.size();
      } else {
        switch (_pointsCount) {
          case 1 : { // move gestures
            int xDiff = points[0].pos().x() - _points[0].x();
            int yDiff = points[0].pos().y() - _points[0].y();
            if (xDiff || yDiff) {
              _points[0] = points[0].pos();
              emit pinch(xDiff, yDiff, 1.0, 0.0);
            }
            break;
          }
          case 2 : { // pinch gestures
            QPointF newCenter = (points[0].pos() + points[1].pos()) / 2;
            double newDistance = distance(points[0].pos(), points[1].pos());
            double newAngle = angle(points[0].pos(), points[1].pos(), newDistance);
            bool needEmit = false;

            // moving check
            int xDiff = newCenter.x() - _oldCenter.x();
            int yDiff = newCenter.y() - _oldCenter.y();
            if (xDiff || yDiff) {
              _oldCenter = newCenter;
              needEmit |= true;
            }

            double scale = distance(points[0].pos(), points[1].pos()) / _oldDistance;
            if (scale <= 0.98 || scale >= 1.02) {
              _oldDistance = newDistance;
              needEmit |= true;
            } else {
              scale = 1.0;
            }

            double angleDiff = newAngle - _oldAngle;
            if (angleDiff >= M_PI)
              angleDiff -= M_PI*2;
            else if (angleDiff <= -M_PI)
              angleDiff += M_PI*2;
            if (fabs(angleDiff) >= (M_PI / 180.0)) {
              _oldAngle = newAngle;
              needEmit |= true;
            } else {
              angleDiff = 0.0;
            }

            if (needEmit)
              emit pinch(xDiff, yDiff, scale, angleDiff);
            break;
          }
          case 3 : { // 3-finger move gestures
            double delta = 0.0;
            for (int i = 0; i < 3; i++)
              delta += points[i].pos().y() - _points[i].y();
            delta /= 3.0;
            if (delta > 1.0 || delta < -1.0) {
              _points[0] = points[0].pos();
              _points[1] = points[1].pos();
              _points[2] = points[2].pos();
              emit drag3f((int)delta);
            }
            break;
          }
        }
      }

      break;
    }

    default :
      return false;
  }

  return true;
}

bool GestureRecognizer::process(QMouseEvent *event)
{
  if (_pointsCount == 0) {
    if (event->buttons() & Qt::LeftButton) {
      if (event->type() & QEvent::MouseButtonPress) {
        _mousePoint = event->pos();
      } else if (event->type() & QEvent::MouseMove) {
        int deltaX = event->x() - _mousePoint.x();
        int deltaY = event->y() - _mousePoint.y();
        _mousePoint = event->pos();
        emit pinch(deltaX, deltaY, 1.0, 0.0);
      }
    }
    
    return true;
  }
  
  return false;
}


bool GestureRecognizer::process(QWheelEvent *event)
{
  if (_pointsCount == 0) {
    emit pinch(0, 0, event->delta() > 0 ? 1.1 : 0.9, 0.0);
    return true;
  }
  
  return false;
}
