cmake_minimum_required(VERSION 3.0.0)
project(ScanLine VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include_directories(include)
include_directories(headers)
link_directories(lib)

set(SOURCE_FILES)

add_executable(ScanLine main.cpp glad.c)
target_link_libraries(${PROJECT_NAME} glfw3)