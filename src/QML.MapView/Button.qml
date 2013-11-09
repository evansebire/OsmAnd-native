import QtQuick 2.0

Label {
  id: button
  signal clicked

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
      button.clicked()
    }
  }

  states: State {
    name: "pressed";
    when: mouseArea.pressed == true
    PropertyChanges { target: shade; opacity: 0.4 }
  } 
}
