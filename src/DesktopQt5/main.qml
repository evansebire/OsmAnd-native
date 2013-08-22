import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import "QML.MapView"

 
ApplicationWindow {
  visible: true
  title: "OsmAnd"
  menuBar: MenuBar {
    Menu {
      title: "OsmAnd"
      MenuItem { text: "Settings" }
      MenuItem {
        text: "Exit"
        onTriggered: Qt.quit()
      }
    }
    
    Menu {
      title: "View"
      MenuItem { text: "Search" }
      MenuItem { text: "Favorites" }
    }
    
    Menu {
      title: "Help"
      MenuItem { text: "About" }
    }
  }

  toolBar: ToolBar {
    RowLayout {
      ToolButton { }
      ToolButton { }
      ToolButton { }
    }
  }
  
  MapView {
    anchors.fill: parent
  }
}
