# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(OmniCalibration)

if(DEFINED OMNIDOME_PATH)
    MESSAGE(STATUS "Omnidome Path: ${QT_PATH}")
    setup_qt(5.5 ${QT_PATH})
else()
    SET(OMNIDOME_PATH "${CMAKE_SOURCE_DIR}/../omnidome")
endif()

include(${OMNIDOME_PATH}/cmake-files/base.cmake)

import(
  plugin
  compiler_flags
  qt
  gl
  cpack
)

################################

      set(QT_MODULES "")
      LIST(APPEND QT_MODULES "Core")
      LIST(APPEND QT_MODULES "Gui")
    #  LIST(APPEND QT_MODULES "Widgets")
      LIST(APPEND QT_MODULES "OpenGL")
      LIST(APPEND QT_MODULES "Quick")
      LIST(APPEND QT_MODULES "Qml")

if(DEFINED QT_PATH)
    MESSAGE(STATUS "Qt Path: ${QT_PATH}")
    setup_qt(5.6 ${QT_PATH})
else()
    detect_qt()
endif()

if(NOT QT_FOUND)
    MESSAGE(FATAL_ERROR "Qt Framework was not found. You might set QT_PATH manually.")
    return()
endif()


########### BUILD_TARGETS ############

include_directories(./include)
common_cxx_flags()

######## Examples #########
function(build_example EXAMPLE)
  SET(EXAMPLE_DIR ${CMAKE_SOURCE_DIR}/src/examples/${EXAMPLE})
  if(EXISTS ${EXAMPLE_DIR}/build.cmake)
      include(${EXAMPLE_DIR}/build.cmake)
  endif()

  FILE(GLOB_RECURSE example_headers ${EXAMPLE_DIR}/*.h )
  FILE(GLOB_RECURSE example_forms ${EXAMPLE_DIR}/ui/*.ui )
  FILE(GLOB_RECURSE example_resources ${EXAMPLE_DIR}/*.qrc )
  FILE(GLOB_RECURSE example_sources ${EXAMPLE_DIR}/*.cpp )

  QT5_WRAP_CPP(example_headers_moc ${example_headers})
  QT5_ADD_RESOURCES(example_resources_rcc ${example_resources})
  QT5_GENERATE_UI(example_forms_headers ${example_forms})
  SET(example_src ${example_sources} ${example_headers_moc} ${example_forms_headers} ${example_resources_rcc})

  include_directories(${${EXAMPLE}_INCLUDE})

  ADD_EXECUTABLE(${EXAMPLE} ${example_src} )
  TARGET_LINK_LIBRARIES(${EXAMPLE} ${Qt5_LIBRARIES} ${omni_gl_LIBRARIES} ${${EXAMPLE}_libs} omni )

#  foreach(QT_MODULE ${QT_MODULES})
#    qt5_use_modules(${EXAMPLE} ${QT_MODULE})
#  endforeach()
endfunction()

build_example(CubeCam)
build_example(OmniPlayer)
build_example(Nodes)
