
# Build options for sample app

#target_compile_definitions(sampleApp PUBLIC "-DOMNIC_USE_QT_GL")

MESSAGE(STATUS "Hello")

SET_TARGET_PROPERTIES(sampleApp PROPERTIES COMPILE_FLAGS -DOMNIC_USE_QT_GL)

