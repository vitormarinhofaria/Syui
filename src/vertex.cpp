#include "vertex.hpp"

std::array<D3D11_INPUT_ELEMENT_DESC, 3> Syui::Vertex::GetInputLayout()
{
    std::array<D3D11_INPUT_ELEMENT_DESC, 3> ied = {{
        {"POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    }};
    return ied;
}