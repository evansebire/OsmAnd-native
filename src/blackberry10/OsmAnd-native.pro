QMAKE_CXXFLAGS += -Y _gpp -Wc,-std=gnu++0x,-DOSMAND_OPENGLES2_RENDERER_SUPPORTED,-DOSMAND_TARGET_OS_qnx
QMAKE_LFLAGS += -Y _gpp

INCLUDEPATH += ../include $(OSMAND_ROOT_DIRECTORY)/core/include

SOURCES += main.cpp
SOURCES += \
  ../clientcore/GestureRecognizer.cpp \
  ../clientcore/MapComponent.cpp \
  ../clientcore/Serializer.cpp

HEADERS += \
  ../include/GestureRecognizer.h \
  ../include/MapComponent.h \
  ../include/OsmAndConfig.h \
  ../include/Serializer.h


debug {
  LIBS += -L$(OSMAND_ROOT_DIRECTORY)/binaries/qnx/arm/Debug
}

release {
  LIBS += -L$(OSMAND_ROOT_DIRECTORY)/binaries/qnx/arm/Release
}

LIBS += \
  -lgif \
  -lfontconfig \
  -lfreetype \
  -lOsmAndCore_static \
  -lskia_static \
  -lgdal_static \
  -lpng_static \
  -ljpeg_static \
  -lz_static

QT += qml quick

OTHER_FILES += \
  bar-descriptor.xml
