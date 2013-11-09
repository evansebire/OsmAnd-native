#include <QtGui/QGuiApplication>
#include <QQuickView>

#include "MapComponent.h"
#include "OsmAndConfig.h"

#include <OsmAndCore.h>

class EventFilter : public QObject {
private:
  QObject *_rootObject;

public:
  EventFilter(QObject *rootObject) : _rootObject(rootObject) {}
  bool eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::PlatformPanel) {
      QMetaObject::invokeMethod(_rootObject, "systemMenu");
      return true;
    }

    return QObject::eventFilter(obj, event);
  }
};


int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  OsmAnd::InitializeCore();

  qmlRegisterType<MapComponent>("MapComponent", 1, 0, "MapComponent");
  qmlRegisterType<OsmAndCfgMap>("OsmAndCfgMap", 1, 0, "OsmAndCfgMap");
  qmlRegisterType<OsmAndCfgInterface>("OsmAndCfgInterface", 1, 0, "OsmAndCfgInterface");
  
  QQuickView viewer;
  viewer.setResizeMode(QQuickView::SizeRootObjectToView);
  viewer.setSource(QUrl("app/native/qml/main.qml"));
  viewer.showFullScreen();

  EventFilter filter(viewer.rootObject());
  app.installEventFilter(&filter);

  return app.exec();
}
