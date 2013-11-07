#ifndef __XMLSERIALIZER_H_
#define __XMLSERIALIZER_H_

#include <QObject>
class QFile;


#define Q_PROPERTY_AUTO(type, name)\
type name; \
type get##name() { return name; } \
void set##name(type value) { name = value; } \
Q_PROPERTY(type name READ get##name WRITE set##name);

#define Q_PROPERTY_AUTO_NOTIFY(type, name)\
type name; \
type get##name() { return name; } \
void set##name(type value) { name = value; emit name##Changed(value); } \
Q_PROPERTY(type name READ get##name WRITE set##name NOTIFY name##Changed);

class Serializer : public QObject {
  Q_OBJECT
  
protected:
  QFile *_file;
  
public:
  Serializer() : _file(0) {}
  virtual ~Serializer();
  
  // NOTE: May be possible to do this using QML/Javascript only?
  Q_INVOKABLE static QString appRootDirectory();
  
  Q_INVOKABLE void linkWith(QString file);
  
public slots:  
  virtual void load() = 0;
  virtual void dump() = 0;
  
signals:
  void updated();
};


class BinarySerializer : public Serializer {
public:
  virtual void load();
  virtual void dump();
};


class XmlSerializer : public Serializer {
public:
  virtual void load();
  virtual void dump();
};


  

#endif //__XMLSERIALIZER_H_

