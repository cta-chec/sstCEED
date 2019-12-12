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
#set(HEADER_LIST "${sstCEED_SOURCE_DIR}/include/sstCEED/WaveformDataPacket.h")
add_library(${PROJECT_NAME} src/WaveformDataPacket.cc ${HEADER_LIST})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})

# tests
add_library(test_main OBJECT tests/test_main.cc)
add_executable(test_${PROJECT_NAME} tests/WaveformDataPacket.cc $<TARGET_OBJECTS:test_main>)
add_test(NAME test_${PROJECT_NAME} COMMAND test_${PROJECT_NAME})
#add_executable(tests tests/test_main.cc tests/WaveformDataPacket.cc ${HEADER_LIST})
target_include_directories(test_${PROJECT_NAME} PUBLIC include tests)
target_compile_features(test_${PROJECT_NAME} PRIVATE cxx_std_17)
#target_link_libraries(tests PRIVATE doctest:: doctest)
#target_link_libraries(test1 doctest)

# data files
file(GLOB DATA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/${PROJECT_NAME}/*")
file(COPY ${DATA_FILES} DESTINATION ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/)
install(DIRECTORY "${CMAKE_BINARY_DIR}/share" DESTINATION .)

#add_subdirectory(src)
#add_subdirectory(tests)

#pybind11_add_module(my_lib_module src/pybind/my_lib_module.cpp)
#target_link_libraries(my_lib_module PRIVATE my_lib)
