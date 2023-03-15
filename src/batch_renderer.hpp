#pragma once

#include <vector>

#include <d3d11.h>

#include "texture.hpp"
#include "shader.hpp"
#include "vertex.hpp"

namespace Syui
{
    struct BatchRenderer
    {
        ID3D11Buffer *m_buffer = nullptr;
        ID3D11InputLayout* m_input_layout = nullptr;

        std::vector<Vertex> m_vertices;
        Shader m_shader;
        Texture m_texture_atlas;

        BatchRenderer(Shader shader, Texture texture_atlas);
        BatchRenderer();
        ~BatchRenderer();
        void DrawQuad(Vec2 pos, Vec2 size, Vec4 uv, Vec4 color);
        void BeginBatch();
        void Dispatch();
    };
}