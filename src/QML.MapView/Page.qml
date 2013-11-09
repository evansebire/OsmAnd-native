import QtQuick 2.0

Item {
  property bool hasNavigationbar: true
  property PageStack stack: parent
  
  width: parent.width
  height: hasNavigationbar ? parent.height - stack.navigationBar.height : parent.height
  visible: false
}
