#pragma once
#include "syui.hpp"

#include <array>
#include <d3d11.h>

namespace Syui
{
    struct Vertex{
        Vec2 pos;
        Vec2 uv;
        Vec4 color;

        static std::array<D3D11_INPUT_ELEMENT_DESC, 3> GetInputLayout();
    };
}