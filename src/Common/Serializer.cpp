#include "Serializer.h"
#include <QDataStream>
#include <QFile>
#include <QMetaProperty>


Serializer::Serializer(QString file)
{
  _file = new QFile(file);
  _file->open(QFile::OpenModeFlag::ReadWrite);
}


Serializer::~Serializer()
{
  delete _file;
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
