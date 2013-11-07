#include "Serializer.h"
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QMetaProperty>


Serializer::~Serializer()
{
  delete _file;
}

QString Serializer::appRootDirectory()
{
  // NOTE: only for desktop platforms
#ifdef OSMAND_TARGET_OS_qnx
  return QDir::homePath();
#else
  QDir directory(QDir::home());
  if (!directory.exists(".osmand"))
    directory.mkdir(".osmand");
  directory.cd(".osmand");
  return directory.absolutePath();
#endif
}


void Serializer::linkWith(QString file)
{
  if (_file)
    delete _file;
  
  _file = new QFile(file);
  _file->open(QIODevice::ReadWrite);
  load();
}


void BinarySerializer::load()
{
  _file->reset();
  QDataStream stream(_file);
  for (int i = 0, ie = this->metaObject()->propertyCount(); i < ie; i++) {
    QMetaProperty property = this->metaObject()->property(i);
    if (strcmp(property.name(), "objectName") == 0)
      continue;
    
    QVariant value;
    stream >> value;
    property.write(this, value);
  }
  
  emit updated();
}

void BinarySerializer::dump()
{
  _file->reset();  
  QDataStream stream(_file);  
  for (int i = 0, ie = this->metaObject()->propertyCount(); i < ie; i++) {
    QMetaProperty property = this->metaObject()->property(i);
    if (strcmp(property.name(), "objectName") == 0)
      continue;
    
    stream << property.read(this);
  }  
}



void XmlSerializer::load()
{
  // TODO: implement
}

void XmlSerializer::dump()
{
  // TODO: implement
}
