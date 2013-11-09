import QtQuick 2.0

Item {
  id: label
  property alias text: text.text
  property alias textColor: text.color
  property alias fontSize: text.font.pixelSize
  property url iconSource

  implicitWidth: text.implicitWidth + image.implicitWidth
  implicitHeight: Math.max(text.implicitHeight, image.implicitHeight)
  
  BorderImage {
    id: image
    source: label.iconSource
    border { left: 10; top: 10; right: 10; bottom: 10 }
  }
  
  Text {
    id: text
    anchors {
      left: image.right;
      verticalCenter: parent.verticalCenter
    }
  }
}
