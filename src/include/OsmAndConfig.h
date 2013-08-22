#ifndef __OSMANDCONFIG_H_
#define __OSMANDCONFIG_H_

#include "Serializer.h"

struct OsmAndConfigOp : public BinarySerializer {
  Q_OBJECT
  
public:  
  Q_PROPERTY_AUTO(qint32, X);
  Q_PROPERTY_AUTO(qint32, Y);
  Q_PROPERTY_AUTO(float, Zoom);
  Q_PROPERTY_AUTO(float, Azimuth);
  Q_PROPERTY_AUTO(float, ElevationAngle);

  OsmAndConfigOp(QString file) : BinarySerializer(file),
    X(1255039468),
    Y(624055097),
    Zoom(7.5f),
    Azimuth(0.0f),
    ElevationAngle(90.0f) {}
};

struct OsmAndConfig : public XmlSerializer {
  Q_OBJECT
  
public:
  OsmAndConfigOp opData;
  
  OsmAndConfig(QString file, QString opFile) :
    XmlSerializer(file), opData(opFile) {
      opData.load();
      load();
    }
};

#endif //__OSMANDCONFIG_H_
