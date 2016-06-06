# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(OmniCalibration)

# Initialize cm8kr build system system
SET(CM8KR_SUMMARY "OMNIC calibration API")
SET(CM8KR_VENDOR "CR8TR GmbH")
SET(CM8KR_VENDOR_SHORT "cr8tr")
SET(CM8KR_VENDOR_EMAIL "office@cr8tr.org")
SET(CM8KR_URL "omnido.me / cr8tr.org")

include(../cm8kr/base.cmake)

cm8kr_import(
  doxygen
  qt
  gl
  plugin
  cpack
  test
)

cm8kr_set_version(1 0 0)

######################################

set(QT_MODULES "")
LIST(APPEND QT_MODULES "Core")
LIST(APPEND QT_MODULES "Gui")
LIST(APPEND QT_MODULES "Widgets")
LIST(APPEND QT_MODULES "OpenGL")

########### BUILD_TARGETS ############

cm8kr_add_shared_library(omnic_qt ${CMAKE_SOURCE_DIR}/src/lib/qt ) 

cm8kr_add_executable(sampleApp ${CMAKE_SOURCE_DIR}/src/sample )

cm8kr_add_test(Serialization "")



