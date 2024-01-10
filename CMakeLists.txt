cmake_minimum_required(VERSION 3.27)
project(RenderEngine C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_DIR Source)
file(GLOB SOURCE_FILES ${SOURCE_DIR}/*.c)

if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/Library/SDL2-2.28.5/x86_64-w64-mingw32")
endif (WIN32)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
