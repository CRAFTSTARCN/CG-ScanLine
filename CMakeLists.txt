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

set(SOURCE_FILES headers/Exception.hpp
                 headers/init.h
                 headers/Logger.h
                 headers/RenderableObject.h
                 headers/SimpleShaderCompiler.h
                 headers/InputHandler.h
                 headers/Component.h
                 headers/Window.h
                 headers/ScanLineAlgo.h
                 headers/RepeatedPixel.h
                 headers/GlobalStat.h
                 headers/GizmosLine.h
                 
                 src/Logger.cpp
                 src/RenderableObject.cpp
                 src/SimpleShaderCompiler.cpp
                 src/InputHandler.cpp
                 src/Component.cpp
                 src/Window.cpp
                 src/ScanLineAlgo.cpp
                 src/RepeatedPixel.cpp
                 src/GlobalStat.cpp
                 src/GizmosLine.cpp
                 )

add_executable(ScanLine main.cpp glad.c ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glfw3)