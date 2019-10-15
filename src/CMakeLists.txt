
########################################################################
# Build libraries and executables
########################################################################

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/VersionConfig.h.in"
  "${PROJECT_BINARY_DIR}/VersionConfig.h"
  )
 
# add the executable
add_executable(gammaScout2Mqtt
     gammaScout2Mqtt.cxx
     Config.h
     Config.cxx
     GammaScoutOnline.cxx
     GammaScoutOnline.h
     Serial.cxx
     Serial.h
     GMQTT.cxx
     GMQTT.h
     mongoose.c
     mongoose.h
)

