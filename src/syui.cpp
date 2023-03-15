#include <iostream>
#include "syui.hpp"
#include "context_manager.hpp"
#include "batch_renderer.hpp"
#include "font.hpp"
#include "shader.hpp"

namespace Syui
{
    void Initialize(ID3D11Device *dx_device, ID3D11DeviceContext *dx_context, Vec2 display_size)
    {
        Context *ctx = GetContext();
        ctx->dx_context = dx_context;
        ctx->dx_device = dx_device;

        D3D11_SAMPLER_DESC sampler_desc = {
            .Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
            .AddressU = D3D11_TEXTURE_ADDRESS_CLAMP,
            .AddressV = D3D11_TEXTURE_ADDRESS_CLAMP,
            .AddressW = D3D11_TEXTURE_ADDRESS_CLAMP,
        };

        HRESULT result = ctx->dx_device->CreateSamplerState(&sampler_desc, &ctx->dx_sampler);
        if (FAILED(result))
            std::cout << "Failed to create sampler state\n";
    }

    static Font s_current_font;
    static Shader s_current_font_shader;
    void SetFont(Font font, Shader shader)
    {
        s_current_font = font;
        s_current_font_shader = shader;
    }

    void Text(wchar_t *text, Vec2 pos, Vec2 size, Vec4 color = Vec4{0.0f, 1.0f, 0.0f, 1.0f})
    {
        static BatchRenderer batch(s_current_font_shader, s_current_font.m_texture);
        std::wstring t = text;
        Vec2 pos_offset = {};
        batch.BeginBatch();
        for (wchar_t c : t)
        {
            float wh = 876.0f;
            Glyph glyph = s_current_font.m_glyphs[c];
            Vec4 uv = {glyph.atlasBounds.x / wh, glyph.atlasBounds.y / wh, glyph.atlasBounds.z / wh, glyph.atlasBounds.w / wh};
            batch.DrawQuad(Vec2{.x = pos.x + pos_offset.x, .y = pos.y + pos_offset.y},
                           size, glyph.atlasBounds, color);
            
            if (c == L'\n')
                pos_offset.y += s_current_font.m_font_data["metrics"]["lineHeight"].get<float>() * size.y;
            else
                pos_offset.x += 0.12 + glyph.advance;
        }
        batch.Dispatch();
    }
}