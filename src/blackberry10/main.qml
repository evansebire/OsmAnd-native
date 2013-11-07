import QtQuick 2.0
import OsmAndCfgInterface 1.0
import OsmAndCfgMap 1.0

import "QML.MapView"

Item {
    OsmAndCfgInterface {
      id: interfaceCfg
      Component.onCompleted: linkWith(appRootDirectory() + "/.OsmAndInterface.cfg")
    }

    OsmAndCfgMap {
      id: mapCfg
      Component.onCompleted: linkWith(appRootDirectory() + "/.OsmAndMap.cfg")
    }
    
    Timer {
      interval: 1000; running: true; repeat: true
      onTriggered: mapCfg.dump()
    }
    
    MapView {
      interfaceCfg: interfaceCfg
      mapCfg: mapCfg
      anchors.fill: parent
    }
}
