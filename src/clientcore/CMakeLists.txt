qt5_wrap_cpp(MocSources
  ${CMAKE_SOURCE_DIR}/include/MapComponent.h
  ${CMAKE_SOURCE_DIR}/include/Serializer.h
  ${CMAKE_SOURCE_DIR}/include/GestureRecognizer.h
  ${CMAKE_SOURCE_DIR}/include/OsmAndConfig.h
)

add_library(OsmAndClientCore STATIC
  MapComponent.cpp
  Serializer.cpp
  GestureRecognizer.cpp
  ${MocSources}
)

qt5_use_modules(OsmAndClientCore
  Quick
)
