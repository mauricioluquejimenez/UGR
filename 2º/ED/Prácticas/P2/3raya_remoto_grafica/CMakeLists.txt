cmake_minimum_required(VERSION 3.16)
project(3raya)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 17)
set(BASE_FOLDER ./)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Qt${QT_VERSION_MAJOR}  COMPONENTS Widgets REQUIRED )
find_package(Qt${QT_VERSION_MAJOR}  COMPONENTS  Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS  Core REQUIRED)


set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(TARGET 3raya)
set(HEADERS 
    include/conexion.h 
    include/consola.h 
    include/ficha.h 
    include/juego.h 
    include/jugador.h 
    include/tablero.h 
    include/tablerografico.h 
    include/ventana.h
)

set(SOURCES
    ./src/conexion.cpp 
    ./src/ficha.cpp 
    ./src/juego.cpp 
    ./src/jugador.cpp 
    ./src/main.cpp 
    ./src/tablero.cpp 
    ./src/tablerografico.cpp 
    ./src/ventana.cpp
    ./src/consola.cpp 
)
add_executable(${TARGET} ${SOURCES} ${HEADERS})
target_include_directories(${TARGET} PRIVATE ${BASE_FOLDER}/include)
target_link_libraries(${TARGET} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core PRIVATE Qt${QT_VERSION_MAJOR}::Gui )

#Para arquitectura windows
if (WIN32)
    target_link_libraries(${TARGET} PRIVATE ws2_32)
endif (WIN32)

include(GNUInstallDirs)
#install(TARGETS 3raya
#    BUNDLE DESTINATION .
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})




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
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${TARGET}_autogen"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}
)            
add_custom_target( zip 
 	    COMMAND ${CMAKE_COMMAND} -E make_directory ${ZIP_OUT}
            COMMAND ${CMAKE_COMMAND} -E tar "cvf" "${ZIP_OUT}/${TARGET}.zip" --format=zip ./CMakeLists.txt ./include ./src  ./images ./Doxyfile.in ./README
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/${BASE_FOLDER}
            COMMENT "Generating zip"
            VERBATIM 
            )

