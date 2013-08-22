#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "MapComponent.h"

#include <iostream>
#include <qopengl.h>

int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);

  OsmAnd::InitializeCore();
 
  qmlRegisterType<MapComponent>("MapComponent", 1, 0, "MapComponent");
  QQmlApplicationEngine engine(QUrl("qrc:/OsmAnd/main.qml"));
  
  app.exec();
}
