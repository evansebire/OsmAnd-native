import QtQuick 2.0

Label {
  id: checkbox
  property bool checked: false
  signal clicked
  
  iconSource: checked ? "images/qtg_graf_checkbox_normal_selected.png" : "images/qtg_graf_checkbox_normal_unselected.png"
  
  MouseArea {
    anchors.fill: parent
    onClicked: { checked ^= true; checkbox.clicked(); }
  }
}
