cmake_minimum_required(VERSION 3.13)
project(p04_juego_letras)

set(CMAKE_CXX_STANDARD 14)

set(BASE_FOLDER estudiante)

include_directories(${BASE_FOLDER}/include)

if (EXISTS ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}/src/cantidad_letras.cpp)
  add_executable(cantidad_letras
    ${BASE_FOLDER}/src/cantidad_letras.cpp
    ${BASE_FOLDER}/src/dictionary.cpp
    ${BASE_FOLDER}/src/letters_set.cpp estudiante/src/cantidad_letras.cpp)
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}/src/diccionario.cpp)
  add_executable(diccionario
    ${BASE_FOLDER}/src/diccionario.cpp
    ${BASE_FOLDER}/src/dictionary.cpp)
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}/src/partida_letras.cpp)
  add_executable(partida_letras
    ${BASE_FOLDER}/src/partida_letras.cpp
    ${BASE_FOLDER}/src/letters_set.cpp
    ${BASE_FOLDER}/src/letters_bag.cpp
    ${BASE_FOLDER}/src/solver.cpp
    ${BASE_FOLDER}/src/dictionary.cpp estudiante/src/partida_letras.cpp)
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}/src/palabras_validas_diccionario.cpp)
    add_executable(palabras_validas_diccionario
            ${BASE_FOLDER}/src/palabras_validas_diccionario.cpp
            ${BASE_FOLDER}/src/dictionary.cpp)
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}/src/partida_letras_eficiente.cpp)
    add_executable(partida_letras_eficiente
            ${BASE_FOLDER}/src/partida_letras_eficiente.cpp
            ${BASE_FOLDER}/src/letters_set.cpp
            ${BASE_FOLDER}/src/letters_bag.cpp
            ${BASE_FOLDER}/src/solver.cpp
            ${BASE_FOLDER}/src/dictionary.cpp estudiante/src/partida_letras_eficiente.cpp)
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
    add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
