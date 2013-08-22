import QtQuick 2.0

Item {
  property alias text: buttonText.text
  property string image: ""  
  signal clicked
  id: button
  height: buttonRow.height
  width: buttonRow.width
  opacity: 1
  Row {
    id: buttonRow;
  
    BorderImage {
      id: buttonImage
      source: "images/" + button.image + ".png"; clip: true
      border { left: 10; top: 10; right: 10; bottom: 10 }
    }

    Text {
      id: buttonText
      anchors.verticalCenter: buttonImage.verticalCenter
      font.pixelSize: buttonImage.height*0.66
    }
  }

  Rectangle {
    id: shade
    anchors.fill: buttonRow; radius: 10; color: "blue"; opacity: 0
  }

  MouseArea {
    id: mouseArea
    anchors.fill: buttonRow
    onClicked: {
      button.clicked()
    }
  }

  states: State {
    name: "pressed"; when: mouseArea.pressed == true
    PropertyChanges { target: shade; opacity: .4 }
  } 
}
