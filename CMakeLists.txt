# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(OmniCalibration)

# Initialize cm8kr build system system
SET(CM8KR_SUMMARY "OMNIDOME is a arbitrary surface projection mapping tool built by CR8TR. It allows you to map any physical room and project virtual content back on to it.")
SET(CM8KR_VENDOR "CR8TR GmbH")
SET(CM8KR_VENDOR_SHORT "cr8tr")
SET(CM8KR_VENDOR_EMAIL "office@cr8tr.org")
SET(CM8KR_URL "omnido.me / cr8tr.org")

include(../cm8kr/base.cmake)

import(
  doxygen
  qt
  gl
  plugin
  cpack
)

######################################

set(QT_MODULES "")
LIST(APPEND QT_MODULES "Core")
LIST(APPEND QT_MODULES "Gui")
LIST(APPEND QT_MODULES "Widgets")
LIST(APPEND QT_MODULES "OpenGL")

########### BUILD_TARGETS ############

