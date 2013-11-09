import QtQuick 2.0

Item {
  id: toolButton
  property alias text: text.text
  property alias textColor: text.color
  property url iconSource
  signal clicked
  
  implicitWidth: Math.max(text.implicitWidth, image.implicitWidth)
  implicitHeight: text.implicitHeight + image.implicitHeight
  
  BorderImage {
    id: image
    source: toolButton.iconSource
    border { left: 10; top: 10; right: 10; bottom: 10 }
  }
  
  Text {
    id: text
    anchors {
      top: image.bottom;
      horizontalCenter: parent.horizontalCenter
    }
  }
  
  Rectangle {
    id: shade
    anchors.fill: parent;
    radius: 10;
    color: "blue";
    opacity: 0.0
  }
  
  MouseArea {
    id: mouseArea
    anchors.fill: parent
    onClicked: {
      toolButton.clicked()
    }
  }
  
  states: State {
    name: "pressed";
    when: mouseArea.pressed == true
    PropertyChanges { target: shade; opacity: 0.4 }
  }   
}
