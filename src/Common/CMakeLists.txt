if (Qt5Enabled)
  qt5_wrap_cpp(MocSources
    ${CMAKE_SOURCE_DIR}/include/MapComponent.h
  )

  add_library(OsmAndClientCore STATIC
    ${MocSources}
    MapComponentQt5.cpp
  )

  qt5_use_modules(OsmAndClientCore
    Quick
  )
else ()

endif ()
