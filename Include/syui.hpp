#pragma once

class ID3D11Device;
class ID3D11DeviceContext;

namespace Syui
{
    struct Vec2
    {
        float x = 0.0f;
        float y = 0.0f;
    };
    struct Vec3
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };
    struct Vec4
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    };

    void Initialize(ID3D11Device *dx_device, ID3D11DeviceContext *dx_context, Vec2 display_size);
    
    struct Font;
    struct Shader;
    void SetFont(Font font, Shader shader);
    void BeginFrame();

    // Text must be UTF16 unicode - wchar_t std::wstring
    void Text(wchar_t *text, Vec2 pos, Vec2 size, Vec4 color);
    void EndFrame();
    void Draw();
}