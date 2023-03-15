#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <cstdint>

#include "syui.hpp"
#include "font.hpp"
#include "shader.hpp"

int main(int argc, char **argv)
{
    WNDCLASSEXA wc = {sizeof(WNDCLASSEXA), CS_CLASSDC, DefWindowProcA, 0, 0, GetModuleHandleA(NULL), nullptr, nullptr, nullptr, nullptr, "DX", nullptr};
    RegisterClassExA(&wc);

    HWND window = CreateWindowA("DX", "Test Syui", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720, nullptr, nullptr, wc.hInstance, nullptr);
    ShowWindow(window, SHOW_OPENWINDOW);

    DXGI_SWAP_CHAIN_DESC sd = {0};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 1280;
    sd.BufferDesc.Height = 720;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL feature_levels_request = D3D_FEATURE_LEVEL_11_0;
    uint32_t num_feature_levels_request = 1;
    D3D_FEATURE_LEVEL supported_feature_levels;

    IDXGISwapChain *swap_chain = nullptr;
    ID3D11Device *device = nullptr;
    ID3D11DeviceContext *context = nullptr;
    ID3D11RenderTargetView *rtv = nullptr;
    ID3D11DepthStencilView *rdsv = nullptr;
    ID3D11DepthStencilState *ds_state = nullptr;
    ID3D11BlendState *blend_state = nullptr;

    HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &feature_levels_request, num_feature_levels_request, D3D11_SDK_VERSION, &sd, &swap_chain, &device, &supported_feature_levels, &context);

    ID3D11Texture2D *backbuffer;
    swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backbuffer);
    device->CreateRenderTargetView(backbuffer, NULL, &rtv);
    D3D11_TEXTURE2D_DESC rb_desc = {};
    backbuffer->GetDesc(&rb_desc);
    rb_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    rb_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D *depth_tx = nullptr;
    device->CreateTexture2D(&rb_desc, nullptr, &depth_tx);
    device->CreateDepthStencilView(depth_tx, nullptr, &rdsv);

    D3D11_BLEND_DESC blend_desc = {};
    D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};
    rtbd.BlendEnable = true;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.BlendOp = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0] = rtbd;
    device->CreateBlendState(&blend_desc, &blend_state);

    context->OMSetRenderTargets(1, &rtv, nullptr);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    D3D11_VIEWPORT vp = {.Width = 1280, .Height = 720};
    context->RSSetViewports(1, &vp);

    Syui::Initialize(device, context, Syui::Vec2{.x = 1280, .y = 720});

    Syui::Shader shader = Syui::Shader::FromCodeFile("TextVS.hlsl", "TextPS.hlsl");
    Syui::Font font = Syui::Font("pixels.png", "noto_serif.json");
    Syui::SetFont(font, shader);

    MSG msg;
    bool running = true;
    while (running)
    {
        while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        FLOAT clear[4] = {0.2f, 0.2f, 0.2f, 1.0f};
        context->ClearRenderTargetView(rtv, clear);
        context->ClearDepthStencilView(rdsv, D3D11_CLEAR_DEPTH, 1, 0);
        context->OMSetBlendState(blend_state, nullptr, 0xffffffff);

        const char* t = "Hello";
        wchar_t wt[50];
        MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, t, -1, wt, 50);
        Syui::Text(wt, {-0.8f, 0.8f}, {0.1f, 0.1f}, {1.0f, 1.0f, 1.0f, 1.0f});

        swap_chain->Present(0, 0);
    }
}