import QtQuick 2.0
import MapComponent 1.0

Item {
  property alias mapViewName: map.objectName
  
  MapComponent {
    id: map
    anchors.fill: parent
    onScaleChanged: scaleText.text = scaleFormat(scale)
  }
  
  Row {
    anchors.left: parent.left
    anchors.top: parent.top    
    MVButton { image: "map_compass"}
    MVButton { image: "map_monitoring"}
  }
  
  Row {
    id: ctrlRow
    anchors.right: parent.right
    anchors.top: parent.top
    MVButton { image: "map_config"}
    MVButton { image: "la_backtoloc_disabled"}
  }
  
  Column {
    anchors.right: parent.right
    anchors.top: ctrlRow.bottom
    anchors.topMargin: 32
    //todo: show miles depends on config
    MVButton { image: "widget_intermediate"; text: map.distanceToFirstCP + " km"}
    MVButton { image: "list_destination"; text: map.distance + " km"}
    MVButton { image: "widget_speed"; text: map.speed + " kph"}
    MVButton { image: "widget_altitude"; text: map.altitude + " m"}
    MVButton { image: "widget_time"; text: Qt.formatDateTime(map.timeToDestination, "hh:mm") }
  }
  
  Row {
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    MVButton { image: "map_btn_menu_o"}
  }
  
  Row {
    id: scaleRow
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    MVButton { image: "map_zoom_out_o"; onClicked: map.zoomOut() }
    MVButton { image: "map_zoom_in_o"; onClicked: map.zoomIn() }    
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