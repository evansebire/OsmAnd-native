import QtQuick 2.0
import "PageStack.js" as Engine

Item {
  id: stack
  property Item homePage;
  property Item currentPage: homePage
  property Item navigationBar: navigationBar
  
  function push(page) { return Engine.push(page); }
  function pop() { return Engine.pop(); }
  function clear() { return Engine.clear(); }
  
  Rectangle {
    id: navigationBar
    color: "black"
    width: parent.width
    anchors.top: currentPage ? currentPage.bottom : undefined
    implicitHeight: back.implicitHeight
    visible: currentPage ? currentPage.hasNavigationbar : false
    Button {
      id: back
      text: "<"
      textColor: "white"
      fontSize: 24
      onClicked: stack.pop()
    }
  }
}
