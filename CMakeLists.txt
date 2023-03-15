cmake_minimum_required(VERSION 3.25)

project(Syui)
set(CMAKE_CXX_STANDARD 23)

set(SRCS 
        src/syui.cpp Include/syui.hpp
        src/context_manager.cpp src/context_manager.hpp
        src/texture.cpp src/texture.hpp
        src/font.cpp src/font.hpp
        src/shader.cpp src/shader.hpp
        src/batch_renderer.cpp src/batch_renderer.hpp
        src/vertex.cpp src/vertex.hpp
    )

add_library(Syui STATIC ${SRCS})
target_include_directories(Syui PRIVATE submodules/nlohmann_json/single_include Include submodules/stb)

add_executable(SyuiTest test_program/main.cpp)
target_link_libraries(SyuiTest PRIVATE Syui d3d11 dxgi d3dcompiler)
target_include_directories(SyuiTest PRIVATE Include src submodules/nlohmann_json/single_include submodules/stb)
