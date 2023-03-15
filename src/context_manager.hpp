#pragma once

#include <d3d11.h>
#include <vector>
#include "nlohmann/json.hpp"

#include "batch_renderer.hpp"

namespace Syui
{
    struct Context
    {
        ID3D11Device *dx_device = nullptr;
        ID3D11DeviceContext *dx_context = nullptr;
        ID3D11SamplerState *dx_sampler = nullptr;
    };

    Context *GetContext();
    ID3D11Device *GetDxDevice();
    ID3D11DeviceContext *GetDxContext();
}
