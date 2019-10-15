cmake_minimum_required (VERSION 2.6)
project (Tutorial)
# The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)

set(D MG_ENABLE_MQTT MG_ENABLE_IPV6)

# add the binary tree to the search path for include files.
include_directories("${PROJECT_BINARY_DIR}")
 
add_subdirectory(src)

