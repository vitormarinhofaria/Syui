#pragma once
#include <d3d11.h>
namespace Syui
{
    struct Shader{
        ID3D11VertexShader* m_vertex_shader;
        ID3D10Blob* m_vertex_blob;
        ID3D11PixelShader* m_pixel_shader;


        /// @brief Create shader from .hlsl source
        /// @param vertex_hlsl path to vertex shader code
        /// @param pixel_hlsl path to pixel shader code
        static Shader FromCodeFile(const char* vertex_hlsl, const char* pixel_hlsl);
        /// @brief Create shader from precompiled shader blobs. Doesn't take ownership of buffers, caller should handle them.
        /// @param vertex_blob buffer with vertex shader blob
        /// @param pixel_blob buffer with pixel shader blob
        static Shader FromBlob(char* vertex_blob, char* pixel_blob);

        Shader(const Shader& other); //Copy Constructor
        Shader& operator=(const Shader& other); //Copy Assignment
        ~Shader();
        Shader();

    private:
        Shader(ID3D11VertexShader* vertex_shader, ID3D10Blob* vertex_blob, ID3D11PixelShader* pixel_shader);

    };
}