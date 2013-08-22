set(Sources Serializer.cpp)

if (Qt5Enabled)
  qt5_wrap_cpp(MocSources
    ${CMAKE_SOURCE_DIR}/include/MapComponent.h
    ${CMAKE_SOURCE_DIR}/include/Serializer.h
  )

  add_library(OsmAndClientCore STATIC
    ${MocSources}
    MapComponentQt5.cpp
    ${Sources}
  )

  qt5_use_modules(OsmAndClientCore
    Quick
  )
else ()

endif ()
