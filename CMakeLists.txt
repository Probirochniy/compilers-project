cmake_minimum_required(VERSION 3.27.7)
project(adi)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC_DIR "src/*.cpp")

add_library(${PROJECT_NAME}_lib ${SRC_DIR})
target_include_directories(${PROJECT_NAME}_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
MESSAGE(STATUS "Connected: ${PROJECT_NAME}_lib")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)

install(TARGETS ${PROJECT_NAME} DESTINATION bin) 