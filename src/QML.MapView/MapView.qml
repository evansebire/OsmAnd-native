import QtQuick 2.0
import MapComponent 1.0

import OsmAndCfgInterface 1.0
import OsmAndCfgMap 1.0

Item {
  id: mapView
  property OsmAndCfgInterface interfaceCfg
  property alias mapCfg: map.config
  
  signal mapViewConfigure
  
  MapComponent {
    id: map
    anchors.fill: parent
  }
  
  Row {
    anchors.left: parent.left
    anchors.top: parent.top    
    Button { iconSource: "images/map_compass.png"}
    Button { iconSource: "images/map_monitoring.png"}
  }
  
  Row {
    id: ctrlRow
    anchors.right: parent.right
    anchors.top: parent.top
    Button { iconSource: "images/map_config.png"; visible: interfaceCfg.CfgButtonVisible; onClicked: mapView.mapViewConfigure()}
    Button { iconSource: "images/la_backtoloc_disabled.png"; visible: interfaceCfg.CurrentPosButtonVisible}
  }
  
  Column {
    anchors.right: parent.right
    anchors.top: ctrlRow.bottom
    anchors.topMargin: 32
    //todo: show miles depends on config
    Button { iconSource: "images/widget_intermediate.png"; text: map.distanceToFirstCP + " km"}
    Button { iconSource: "images/list_destination.png"; text: map.distance + " km"}
    Button { iconSource: "images/widget_speed.png"; text: map.speed + " kph"}
    Button { iconSource: "images/widget_altitude.png"; text: map.altitude + " m"}
    Button { iconSource: "images/widget_time.png"; text: Qt.formatDateTime(map.timeToDestination, "hh:mm") }
  }
  
  Row {
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    Button { iconSource: "images/map_btn_menu_o.png"}
  }
  
  Row {
    id: scaleRow
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    Button { iconSource: "images/map_zoom_out_o.png"; onClicked: map.zoomOut() }
    Button { iconSource: "images/map_zoom_in_o.png"; onClicked: map.zoomIn() }    
  }
  
  Text {
    id: scaleText
    anchors.bottom: scaleRow.top
    anchors.horizontalCenter: scaleRow.horizontalCenter
    font.pixelSize: scaleRow.height*0.33
    text: scaleFormat(map.scale)
  }
  
  function scaleFormat(value) {
    if (value >= 100000) {
      return Math.round(value / 1000) + " km";
    } else if (value >= 1000) {
      return (value / 1000).toFixed(1) + " km"
    } else {
      return Math.round(value) + " m"; 
    }
  }
}