#include "context_manager.hpp"

namespace
{
    Syui::Context m_context;
}
namespace Syui
{
    Context *GetContext()
    {
        return &m_context;
    }

    ID3D11Device *GetDxDevice()
    {
        return m_context.dx_device;
    }
    ID3D11DeviceContext *GetDxContext()
    {
        return m_context.dx_context;
    }
}