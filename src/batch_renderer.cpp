#include "batch_renderer.hpp"
#include "context_manager.hpp"

#define NUMBER_OF_QUADS 512

namespace Syui
{
    static UINT buffer_size;
    static uint32_t quad_count = 0;

    BatchRenderer::BatchRenderer(Shader shader, Texture texture_atlas) : m_shader(shader), m_texture_atlas(texture_atlas)
    {
        ID3D11Device *dx_device = GetDxDevice();

        buffer_size = (sizeof(Vertex) * 6) * NUMBER_OF_QUADS;
        D3D11_BUFFER_DESC desc{};
        desc.ByteWidth = buffer_size;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.StructureByteStride = sizeof(Vertex);

        HRESULT result = dx_device->CreateBuffer(&desc, nullptr, &this->m_buffer);

        auto ied = Vertex::GetInputLayout();
        result = dx_device->CreateInputLayout(ied.data(), ied.size(), m_shader.m_vertex_blob->GetBufferPointer(), m_shader.m_vertex_blob->GetBufferSize(), &m_input_layout);
    }

    BatchRenderer::~BatchRenderer()
    {
        if (m_buffer)
            m_buffer->Release();
    }

    void BatchRenderer::DrawQuad(Vec2 pos, Vec2 size, Vec4 uv, Vec4 color)
    {
        if (quad_count == NUMBER_OF_QUADS - 1)
        {
            this->Dispatch();
            this->BeginBatch();
        }
        std::array<Vertex, 6> vertices = {{
            Vertex{.pos = {.x = (-1.0f * size.x) + pos.x, .y = (1.0f * size.y) + pos.y}, .uv = {.x = uv.x, .y = uv.w}, .color = color},
            Vertex{.pos = {.x = (1.0f * size.x) + pos.x, .y = (1.0f * size.y) + pos.y}, .uv = {.x = uv.z, .y = uv.w}, .color = color},
            Vertex{.pos = {.x = (1.0f * size.x) + pos.x, .y = (-1.0f * size.y) + pos.y}, .uv = {.x = uv.z, .y = uv.y}, .color = color},
            Vertex{.pos = {.x = (-1.0f * size.x) + pos.x, .y = (1.0f * size.y) + pos.y}, .uv = {.x = uv.x, .y = uv.w}, .color = color},
            Vertex{.pos = {.x = (1.0f * size.x) + pos.x, .y = (-1.0f * size.y) + pos.y}, .uv = {.x = uv.z, .y = uv.y}, .color = color},
            Vertex{.pos = {.x = (-1.0f * size.x) + pos.x, .y = (-1.0f * size.y) + pos.y}, .uv = {.x = uv.x, .y = uv.y}, .color = color},
        }};

        for (auto &v : vertices)
        {
            m_vertices.push_back(v);
        }

        quad_count++;
    }

    void BatchRenderer::BeginBatch()
    {
        m_vertices.clear();
        quad_count = 0;
    }

    void BatchRenderer::Dispatch()
    {
        ID3D11DeviceContext *dx_ctx = GetDxContext();
        D3D11_MAPPED_SUBRESOURCE mapped_buffer = {};
        dx_ctx->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);

        Vertex *vertices = static_cast<Vertex *>(mapped_buffer.pData);
        std::memcpy(mapped_buffer.pData, m_vertices.data(), m_vertices.size() * sizeof(Vertex));
        dx_ctx->Unmap(m_buffer, 0);

        dx_ctx->VSSetShader(m_shader.m_vertex_shader, nullptr, 0);
        dx_ctx->PSSetShader(m_shader.m_pixel_shader, nullptr, 0);
        dx_ctx->PSSetShaderResources(0, 1, &m_texture_atlas.m_srv);
        dx_ctx->PSSetSamplers(0, 1, &GetContext()->dx_sampler);
                
        dx_ctx->IASetInputLayout(m_input_layout);
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        dx_ctx->IASetVertexBuffers(0, 1, &m_buffer, &stride, &offset);

        dx_ctx->Draw(quad_count * 6, 0);
    }
}