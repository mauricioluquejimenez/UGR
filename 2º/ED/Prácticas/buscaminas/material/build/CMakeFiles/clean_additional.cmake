# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/buscaminas_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/buscaminas_autogen.dir/ParseCache.txt"
  "buscaminas_autogen"
  )
endif()
