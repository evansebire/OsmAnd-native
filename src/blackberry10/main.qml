import QtQuick 2.1
import OsmAndCfgInterface 1.0
import OsmAndCfgMap 1.0

import "QML.MapView"

Item {
    OsmAndCfgInterface {
      id: interfaceCfg
      Component.onCompleted: linkWith(appRootDirectory() + "/.OsmAndInterface.cfg")
    }

    OsmAndCfgMap {
      id: mapCfg
      Component.onCompleted: linkWith(appRootDirectory() + "/.OsmAndMap.cfg")
    }

    Timer {
      interval: 1000; running: true; repeat: true
      onTriggered: mapCfg.dump()
    }

    PageStack {
      id: stack
      anchors.fill: parent
      homePage: map

      Page {
        id: map
        visible: true
        hasNavigationbar: false
        MapView {
          interfaceCfg: interfaceCfg
          mapCfg: mapCfg
          anchors.fill: parent
          onMapViewConfigure: mapConfig.visible ^= true
        }
      }

      Page {
        id: settings
        Rectangle {
          color: "white"
          anchors.fill: parent
          Text { text: "Settings"; font.pixelSize: 64; anchors.centerIn: parent}
        }
      }

      Page {
        id: search
        Rectangle {
          color: "white"
          anchors.fill: parent
          Text { text: "Search"; font.pixelSize: 64; anchors.centerIn: parent}
        }
      }

      Page {
        id: favorites
        stack: stack
        Rectangle {
          color: "white"
          anchors.fill: parent
          Text { text: "Favorites"; font.pixelSize: 64; anchors.centerIn: parent}
        }
      }
    }

    MultiPointTouchArea {
      anchors.fill: parent;
      enabled: menu.visible || mapConfig.visible
      onReleased: { menu.visible = false; mapConfig.visible = false; }
    }

    Rectangle {
      id: mapConfig
      anchors.centerIn: parent
      width: parent.width * 0.9
      height: parent.height * 0.9
      color: "white"
      visible: false
      MultiPointTouchArea {
        anchors.fill: parent;
      }

      Column {
        CheckBox {
          checked: interfaceCfg.CfgButtonVisible;
          text: qsTr("Interface configuration");
          onClicked: { interfaceCfg.CfgButtonVisible = checked; }
        }

        CheckBox {
          checked: interfaceCfg.CurrentPosButtonVisible;
          text: qsTr("Current displacement");
          onClicked: interfaceCfg.CurrentPosButtonVisible = checked
        }
      }
    }

    Rectangle {
      id: menu
      color: "black"
      width: parent.width
      implicitHeight: menuBar.implicitHeight
      anchors.horizontalCenter: parent
      visible: false

      MultiPointTouchArea {
        anchors.fill: parent;
      }

      Row {
        id: menuBar
        anchors.centerIn: parent

        ToolButton {
          iconSource: "QML.MapView/images/button_icon_map.png";
          text: "Map";
          textColor: "white";
          onClicked: { stack.clear(); menu.visible = false; }
        }

        ToolButton {
          iconSource: "QML.MapView/images/button_icon_settings.png";
          text: "Settings";
          textColor: "white";
          onClicked: { stack.clear(); stack.push(settings); menu.visible = false; }
        }

        ToolButton {
          iconSource: "QML.MapView/images/button_icon_search.png";
          text: "Search";
          textColor: "white";
          onClicked: { stack.clear(); stack.push(search); menu.visible = false; }
        }

        ToolButton {
          iconSource: "QML.MapView/images/button_icon_favorites.png";
          text: "Favorites";
          textColor: "white";
          onClicked: { stack.clear(); stack.push(favorites); menu.visible = false; }
        }
      }
    }

    function systemMenu() {
      menu.visible ^= true;
    }
}
