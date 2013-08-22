#ifndef __XMLSERIALIZER_H_
#define __XMLSERIALIZER_H_

#include <QObject>
class QFile;


#define Q_PROPERTY_AUTO(type, name) \
type name; \
type get##name() { return name; } \
void set##name(type value) { name = value; } \
Q_PROPERTY(type name READ get##name WRITE set##name);


class Serializer : public QObject {
  Q_OBJECT
  
protected:
  QFile *_file;
  
public:
  Serializer(QString file);
  virtual void load() = 0;
  virtual ~Serializer();
  
public slots:
  virtual void dump() = 0;
};


class BinarySerializer : public Serializer {
public:
  BinarySerializer(QString file) : Serializer(file) {}
  virtual void load();
  virtual void dump();
};


class XmlSerializer : public Serializer {
public:
  XmlSerializer(QString file) : Serializer(file) {}
  virtual void load();
  virtual void dump();
};


  

#endif //__XMLSERIALIZER_H_
