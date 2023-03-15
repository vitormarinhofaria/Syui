#pragma once

#include <d3d11.h>

namespace Syui
{
    struct Vec2; //fwd declare

    struct Texture{
        ID3D11Texture2D* m_texture = nullptr;
        ID3D11ShaderResourceView* m_srv = nullptr;

        Texture(unsigned char* bytes, Vec2 size);
        Texture();
        ~Texture();
    };
}