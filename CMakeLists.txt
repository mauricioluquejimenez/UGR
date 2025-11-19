cmake_minimum_required(VERSION 3.13)
project(juego_letras)

set(CMAKE_CXX_STANDARD 14)

set(BASE_FOLDER ./)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(${BASE_FOLDER}/include)


if (EXISTS ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}/src/testdiccionario.cpp)
  add_executable(testdiccionario
    ${BASE_FOLDER}/src/testdiccionario.cpp
    ${BASE_FOLDER}/src/dictionary.cpp)
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}/src/letras.cpp)
  add_executable(letras
    ${BASE_FOLDER}/src/letras.cpp
    ${BASE_FOLDER}/src/letters_set.cpp
    ${BASE_FOLDER}/src/letters_bag.cpp
    ${BASE_FOLDER}/src/solver.cpp
    ${BASE_FOLDER}/src/dictionary.cpp)
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}/src/cantidad_letras.cpp)
  add_executable(cantidad_letras
    ${BASE_FOLDER}/src/cantidad_letras.cpp
    ${BASE_FOLDER}/src/letters_set.cpp
    ${BASE_FOLDER}/src/dictionary.cpp)
endif()
if (EXISTS ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}/src/cifras.cpp)
  add_executable(cifras
    ${BASE_FOLDER}/src/cifras.cpp)
endif()


# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen 
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

#Generar el zip
set(ZIP_OUT ${CMAKE_CURRENT_BINARY_DIR}/zip)
add_custom_target(mrproper 
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${ZIP_OUT}"
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            COMMAND ${CMAKE_COMMAND} -E remove_directory "CMakeFiles"
            COMMAND ${CMAKE_COMMAND} -E remove "CMakeCache.txt"
            COMMAND ${CMAKE_COMMAND} -E remove "CMakeDoxyfile.in"
            COMMAND ${CMAKE_COMMAND} -E remove "CMakeDoxygenDefaults.cmake"
            COMMAND ${CMAKE_COMMAND} -E remove "cmake_install.cmake"
            COMMAND ${CMAKE_COMMAND} -E remove_directory "doc_doxygen"
            COMMAND ${CMAKE_COMMAND} -E remove_directory "letras_autogen"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}
)            
add_custom_target( zip 
 	    COMMAND ${CMAKE_COMMAND} -E make_directory ${ZIP_OUT}
            COMMAND ${CMAKE_COMMAND} -E tar "cvf" "${ZIP_OUT}/${TARGET}.zip" --format=zip ./README ./CMakeLists.txt ./include ./src ./datos ./doc ./Doxyfile.in
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}
            COMMENT "Generating zip"
            VERBATIM 
            )




