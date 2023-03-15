#include "shader.hpp"

#include <iostream>
#include <format>

#include <d3dcompiler.h>
#include <Windows.h>

#include "context_manager.hpp"

namespace Syui
{
    Shader::Shader(ID3D11VertexShader *vertex_shader, ID3D10Blob *vertex_blob, ID3D11PixelShader *pixel_shader) : m_vertex_shader(vertex_shader), m_vertex_blob(vertex_blob), m_pixel_shader(pixel_shader)
    {
    }

    Shader Shader::FromCodeFile(const char *vertex_hlsl, const char *pixel_hlsl)
    {
        ID3D11Device *dx_device = GetDxDevice();

        ID3D11VertexShader *vertex_shader;
        ID3D10Blob *vertex_blob;
        {
            ID3D10Blob *vertex_error;
            wchar_t vertex_path[MAX_PATH];
            int bytes_written = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, vertex_hlsl, -1, vertex_path, MAX_PATH);
            if (!bytes_written)
                std::cout << "Failed to convert path from MB String to WC String\n";

            HRESULT result = D3DCompileFromFile(vertex_path, nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertex_blob, &vertex_error);
            if (FAILED(result))
                std::cout << std::format("Failed to compile {}\n\n{}", vertex_hlsl, static_cast<const char *>(vertex_error->GetBufferPointer()));

            result = dx_device->CreateVertexShader(vertex_blob->GetBufferPointer(), vertex_blob->GetBufferSize(), nullptr, &vertex_shader);
            if(vertex_error)
                vertex_error->Release();
        }

        ID3D11PixelShader *pixel_shader;
        {
            ID3D10Blob *pixel_blob;
            ID3D10Blob *pixel_error;
            wchar_t pixel_path[MAX_PATH];
            int bytes_written = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, pixel_hlsl, -1, pixel_path, MAX_PATH);
            if (!bytes_written)
                std::cout << "Failed to convert path from MB String to WC String\n";

            HRESULT result = D3DCompileFromFile(pixel_path, nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &pixel_blob, &pixel_error);
            if (FAILED(result))
                std::cout << std::format("Failed to compile {}\n\n{}", pixel_hlsl, static_cast<const char *>(pixel_error->GetBufferPointer()));

            result = dx_device->CreatePixelShader(pixel_blob->GetBufferPointer(), pixel_blob->GetBufferSize(), nullptr, &pixel_shader);
            if(pixel_blob)
                pixel_blob->Release();
            if(pixel_error)
                pixel_error->Release();
        }
        return Shader(vertex_shader, vertex_blob, pixel_shader);
    }

    Shader Shader::FromBlob(char *vertex_blob, char *pixel_blob)
    {
        return Shader(nullptr, nullptr, nullptr);
    }

    Shader::Shader(const Shader &other)
    {
        if (!other.m_pixel_shader || !other.m_vertex_shader)
            return;

        this->m_vertex_shader = other.m_vertex_shader;
        this->m_vertex_shader->AddRef();
        this->m_pixel_shader = other.m_pixel_shader;
        this->m_pixel_shader->AddRef();
        this->m_vertex_blob = other.m_vertex_blob;
        this->m_vertex_blob->AddRef();
    }

    Shader &Shader::operator=(const Shader &other)
    {
        if (!other.m_pixel_shader || !other.m_vertex_shader)
            return *this;

        this->m_vertex_shader = other.m_vertex_shader;
        this->m_vertex_shader->AddRef();
        this->m_pixel_shader = other.m_pixel_shader;
        this->m_pixel_shader->AddRef();
        this->m_vertex_blob = other.m_vertex_blob;
        this->m_vertex_blob->AddRef();

        return *this;
    }

    Shader::~Shader()
    {
        if (m_vertex_shader)
            m_vertex_shader->Release();

        if (m_vertex_blob)
            m_vertex_blob->Release();

        if (m_pixel_shader)
            m_pixel_shader->Release();
    }
    Shader::Shader()
    {
    }
}