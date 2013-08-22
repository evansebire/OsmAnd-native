#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>

#include "MapComponent.h"
#include "OsmAndConfig.h"

int main(int argc, char **argv)
{
  OsmAndConfig config(QDir::homePath() + "/.OsmAnd.xml",
                      QDir::homePath() + "/.OsmAndOp.bin");

  QGuiApplication app(argc, argv);
  OsmAnd::InitializeCore();
 
  qmlRegisterType<MapComponent>("MapComponent", 1, 0, "MapComponent");
  QQmlApplicationEngine engine(QUrl("qrc:/OsmAnd/main.qml"));
  for (auto object: engine.rootObjects()) {
    MapComponent *mapComponent = object->findChild<MapComponent*>("mapView");
    if (mapComponent) {
      mapComponent->setConfig(&config);
      break;
    }
  }

  QTimer *cfgSaveTimer = new QTimer();
  QObject::connect(cfgSaveTimer, SIGNAL(timeout()), &config.opData, SLOT(dump()));
  cfgSaveTimer->start(1000);
  
  app.exec();
}
