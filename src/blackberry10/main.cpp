#include <QtGui/QGuiApplication>
#include <QQuickView>

#include "MapComponent.h"
#include "OsmAndConfig.h"

#include <OsmAndCore.h>

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

  return app.exec();
}
