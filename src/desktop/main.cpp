#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>

#include "MapComponent.h"
#include "OsmAndConfig.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  OsmAnd::InitializeCore();

  qmlRegisterType<MapComponent>("MapComponent", 1, 0, "MapComponent");
  qmlRegisterType<OsmAndCfgMap>("OsmAndCfgMap", 1, 0, "OsmAndCfgMap");
  qmlRegisterType<OsmAndCfgInterface>("OsmAndCfgInterface", 1, 0, "OsmAndCfgInterface");
  QQmlApplicationEngine engine(QUrl("qrc:/OsmAnd/main.qml"));
  app.exec();
}
