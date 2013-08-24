import QtQuick 2.1
import QtQuick.Controls 1.0

import OsmAndCfgInterface 1.0

Item {
  property OsmAndCfgInterface config
  
  GroupBox {
    title: qsTr("Package selection")
    Column {
      spacing: 2
      CheckBox { text: qsTr("Interface configuration"); checked: config.CfgButtonVisible; onClicked: config.CfgButtonVisible = checked }
      CheckBox { text: qsTr("Current displacement"); checked: config.CurrentPosButtonVisible; onClicked: config.CurrentPosButtonVisible = checked }
    }
  }
}
