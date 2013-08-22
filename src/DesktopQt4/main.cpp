#include <QApplication>
#include <QDeclarativeView>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  
  QDeclarativeView viewer;
  
  viewer.setSource(app.applicationDirPath() + "/OsmAnd_native.qml/main.qml");
  viewer.showMaximized();
  
  app.exec();
}
