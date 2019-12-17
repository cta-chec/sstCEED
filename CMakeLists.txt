cmake_minimum_required(VERSION 3.11...3.16)

project(sstCEED VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Save executables to bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Dependencies
find_package(pybind11 REQUIRED)
find_package(Doxygen)
if(Doxygen_FOUND)
    add_subdirectory(docs)
else()
    message(STATUS "Doxygen not found, not building docs")
endif()

include(CTest)

# src
file(GLOB HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/*")
set(HEADER_LIST
    "${sstCEED_SOURCE_DIR}/include/sstCEED/WaveformDataPacket.h"
    "${sstCEED_SOURCE_DIR}/include/sstCEED/Waveform.h"
)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCES)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADER_LIST})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})

# pybind
pybind11_add_module(${PROJECT_NAME}py pybind/WaveformDataPacket.cc)
target_link_libraries(${PROJECT_NAME}py PRIVATE ${PROJECT_NAME})

# ctests
add_library(test_main OBJECT ctests/test_main.cc)

add_executable(test_WaveformDataPacket ctests/WaveformDataPacket.cc $<TARGET_OBJECTS:test_main>)
add_test(NAME test_WaveformDataPacket COMMAND test_WaveformDataPacket)
target_link_libraries(test_WaveformDataPacket ${PROJECT_NAME})
target_include_directories(test_WaveformDataPacket PUBLIC ctests)
target_compile_features(test_WaveformDataPacket PRIVATE cxx_std_11)

add_executable(test_Waveform ctests/Waveform.cc $<TARGET_OBJECTS:test_main>)
add_test(NAME test_Waveform COMMAND test_Waveform)
target_link_libraries(test_Waveform ${PROJECT_NAME})
target_include_directories(test_Waveform PUBLIC ctests)
target_compile_features(test_Waveform PRIVATE cxx_std_11)

# data files
file(GLOB DATA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/${PROJECT_NAME}/*")
file(COPY ${DATA_FILES} DESTINATION ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/)
install(DIRECTORY "${CMAKE_BINARY_DIR}/share" DESTINATION .)

# Compilation options
target_compile_options(${PROJECT_NAME} PUBLIC -O2 -Wall -pedantic -Werror -Wextra)



#add_subdirectory(src)
#add_subdirectory(ctests)

#pybind11_add_module(my_lib_module src/pybind/my_lib_module.cpp)
#target_link_libraries(my_lib_module PRIVATE my_lib)
