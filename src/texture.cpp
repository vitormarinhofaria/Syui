#include <iostream>
#include <format>

#include "texture.hpp"
#include "syui.hpp"
#include "context_manager.hpp"

namespace Syui
{

    Texture::Texture(unsigned char *bytes, Vec2 size)
    {
        ID3D11Device *dx_dev = GetDxDevice();
        D3D11_TEXTURE2D_DESC desc = {0};
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Width = size.x;
        desc.Height = size.y;
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA init_data = {.pSysMem = bytes, .SysMemPitch = (UINT)desc.Width * 4};
        HRESULT result = dx_dev->CreateTexture2D(&desc, &init_data, &m_texture);
        if (FAILED(result))
            std::cout << std::format("Failed to create Texture. Error: {}\n", result);

        result = dx_dev->CreateShaderResourceView(m_texture, NULL, &m_srv);
        if (FAILED(result))
            std::cout << std::format("Failed to create Texture Resource View. Error {}\n", result);
    }
    Texture::Texture()
    {
    }
    Texture::~Texture()
    {
        if (m_srv)
        {
            // m_srv->Release();
        }

        if (m_texture)
        {
            // m_texture->Release();
        }
    }
}