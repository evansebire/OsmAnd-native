import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.0

import OsmAndCfgInterface 1.0
import OsmAndCfgMap 1.0

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

  OsmAndCfgInterface {
    id: interfaceCfg
    Component.onCompleted: linkWith(configDir() + "/.OsmAndInterface.cfg")
  }
  
  OsmAndCfgMap {
    id: mapCfg 
    Component.onCompleted: linkWith(configDir() + "/.OsmAndMap.cfg")
  }
  
  Timer {
    interval: 1000; running: true; repeat: true
    onTriggered: mapCfg.dump()
  }
  
  Window {
    id: viewCfgDialog
    InterfaceConfig {
      config: interfaceCfg
    }
  }

  MapView {
    interfaceCfg: interfaceCfg
    mapCfg: mapCfg
    anchors.fill: parent
    onMapViewConfigure: viewCfgDialog.visible = true
  }
}
