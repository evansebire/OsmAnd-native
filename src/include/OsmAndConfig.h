#ifndef __OSMANDCONFIG_H_
#define __OSMANDCONFIG_H_

#include "Serializer.h"

struct OsmAndCfgMap : public BinarySerializer {
  Q_OBJECT
  
public:  
  Q_PROPERTY_AUTO(qint32, X);
  Q_PROPERTY_AUTO(qint32, Y);
  Q_PROPERTY_AUTO(float, Zoom);
  Q_PROPERTY_AUTO(float, Azimuth);
  Q_PROPERTY_AUTO(float, ElevationAngle);
  
  OsmAndCfgMap() : BinarySerializer(),
    X(1255039468),
    Y(624055097),
    Zoom(7.5f),
    Azimuth(0.0f),
    ElevationAngle(90.0f) {}
};

struct OsmAndCfgInterface : public XmlSerializer {
  Q_OBJECT
  
signals:
  void CfgButtonVisibleChanged(bool);
  void CurrentPosButtonVisibleChanged(bool);
  
public:
  Q_PROPERTY_AUTO_NOTIFY(bool, CfgButtonVisible);
  Q_PROPERTY_AUTO_NOTIFY(bool, CurrentPosButtonVisible);
  
  OsmAndCfgInterface() : XmlSerializer(),
    CfgButtonVisible(true),
    CurrentPosButtonVisible(true) {}
};

#endif //__OSMANDCONFIG_H_
