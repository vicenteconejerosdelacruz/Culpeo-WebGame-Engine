// #include "pch.h"
#include "Renderer.h"
// #include <DirectXHelper.h>
#include <DeviceUtils/WebGpuDevice/Builder.h>
// #include <DeviceUtils/D3D12Device/Builder.h>
// #include <DeviceUtils/D3D12Device/Interop.h>
// #include <DeviceUtils/ConstantsBuffer/ConstantsBuffer.h>
// #include <DeviceUtils/RenderToTexture/RenderToTexture.h>
#include <RenderPass/RenderPass.h>
#include <Camera/Camera.h>
#include <emscripten.h>

// using namespace Templates::RenderPass;
//using namespace DeviceUtils;

// #if defined(_DEBUG)
// CComPtr<ID3D12Debug1> debugController;
// CComPtr<ID3D12DebugDevice1> debugDevice;
// #endif

// unsigned int backBufferIndex;
extern std::unique_ptr<JRenderer> renderer;
extern RECT hWndRect;


//CREATE
// void JRenderer::Initialize(HWND coreHwnd) {
void JRenderer::Initialize() {

	instance = DeviceUtils::CreateInstance();
	adapter = DeviceUtils::GetAdapter(instance);
	device = DeviceUtils::CreateDevice(instance, adapter);

// 	UpdateViewportPerspective();
	CreateSwapChainPass();

// 	fence = CreateFence(d3dDevice, "Renderer");
// 	fenceEvent = CreateEventHandle();
}
void JRenderer::CreateWindow()
{
    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // 1. Obtenemos las dimensiones físicas reales del canvas desde JavaScript (con soporte de Pixel Ratio)
    int canvasWidth = EM_ASM_INT({
        var canvas = document.getElementById('canvas') || document.querySelector('canvas');
        if (!canvas) return 512;
        var dpr = window.devicePixelRatio || 1;
        // Opcional: aseguramos que el tamaño interno del canvas coincida con su renderizado CSS * DPR
        var displayWidth = Math.round(canvas.clientWidth * dpr);
        var displayHeight = Math.round(canvas.clientHeight * dpr);
        if (canvas.width !== displayWidth || canvas.height !== displayHeight) {
            canvas.width = displayWidth;
            canvas.height = displayHeight;
        }
        return canvas.width;
    });

    int canvasHeight = EM_ASM_INT({
        var canvas = document.getElementById('canvas') || document.querySelector('canvas');
        if (!canvas) return 512;
        var dpr = window.devicePixelRatio || 1;
        return Math.round(canvas.clientHeight * dpr);
    });

    // 2. Creamos la ventana de GLFW usando las dimensiones exactas del canvas físico
    window = glfwCreateWindow(canvasWidth, canvasHeight, "WebGPU window", nullptr, nullptr);

    // 3. Registrar el callback para cuando el navegador cambie de tamaño el canvas
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        renderer->ResizeSwapChain(width, height);
    });
    
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    hWndRect.right = windowWidth;
    hWndRect.bottom = windowHeight;
}

void JRenderer::ResizeSwapChain(int width, int height)
{
    if (width == 0 || height == 0) return; // Evitar tamaños inválidos si se minimiza

    windowWidth = width;
    windowHeight = height;
    hWndRect.right = width;
    hWndRect.bottom = height;
    PrintRect("hWndRect", hWndRect);
    ConfigureSurface(windowWidth, windowHeight);
}

void JRenderer::CreateSurface()
{
    surface = DeviceUtils::CreateSurface(instance, window);
    ConfigureSurface(windowWidth, windowHeight);
}

void JRenderer::ConfigureSurface(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0) return; // Evitar tamaños inválidos si se minimiza

    wgpu::SurfaceCapabilities capabilities;
    surface.GetCapabilities(adapter, &capabilities);
    format = capabilities.formats[0];

    wgpu::SurfaceConfiguration config{.device = device,
                                      .format = format,
                                      .width = width,
                                      .height = height,
                                      .presentMode = wgpu::PresentMode::Fifo
                                  };
    surface.Configure(&config);
}

void JRenderer::CreateSwapChainPass()
{
	using namespace Templates;
	swapChainPass = CreateRenderPassInstance(CameraID(), GetRenderPassUUIDByName("simplePass"), 0);
}

unsigned int JRenderer::GetBackBufferIndex()
{
	return backBufferIndex;
}

// void JRenderer::UpdateViewportPerspective() {
// 	RECT rect;
// 	GetWindowRect(hwnd, &rect);
// 	int width = rect.right - rect.left;
// 	int height = rect.bottom - rect.top;

// 	scissorRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
// 	screenViewport = { 0.0f, 0.0f, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };
// }

// void JRenderer::Resize(unsigned int width, unsigned int height) {
// 	using namespace DeviceUtils;

// 	Flush();

// 	for (unsigned int i = 0; i < numFrames; ++i)
// 	{
// 		frameFenceValues[i] = frameFenceValues[backBufferIndex];
// 	}

// 	scissorRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
// 	screenViewport = { 0.0f, 0.0f, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };

// 	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
// 	DX::ThrowIfFailed(swapChain->GetDesc(&swapChainDesc));
// 	DX::ThrowIfFailed(swapChain->ResizeBuffers(numFrames, width, height,
// 		swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));

// 	backBufferIndex = swapChain->GetCurrentBackBufferIndex();
// }

// static std::mutex commandsMutex;
// void JRenderer::ExecuteCommands(CComPtr<ID3D12GraphicsCommandList2>& commandList, std::function<void()> callback)
// {
// 	ID3D12CommandList* const commandLists[] = { commandList };
// 	commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
// 	if (callback)
// 	{
// 		std::lock_guard<std::mutex> lock(commandsMutex);
// 		executionCallback.push_back(callback);
// 	}
// }

// void JRenderer::Present() {
// 	using namespace DeviceUtils;

// 	UINT presentFlags = 0;

// 	// Solo agregar el flag de Tearing si la GPU/OS lo soporta
// 	if (supportsTearing)
// 	{
// 		BOOL isFullscreen = FALSE;
// 		// Verificar que el SwapChain NO esté en estado pantalla completa exclusivo de DirectX
// 		if (SUCCEEDED(swapChain->GetFullscreenState(&isFullscreen, nullptr)))
// 		{
// 			if (!isFullscreen)
// 			{
// 				presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
// 			}
// 		}
// 	}

// 	// Presentar usando los flags calculados dinámicamente
// 	DX::ThrowIfFailed(swapChain->Present(0, presentFlags));

// 	frameFenceValues[backBufferIndex] = Signal(commandQueue, fence, fenceValue);
// 	backBufferIndex = swapChain->GetCurrentBackBufferIndex();

// 	//make the CPU to wait for the GPU to finish the current processing
// 	WaitForFenceValue(fence, frameFenceValues[backBufferIndex], fenceEvent);
// 	if (executionCallback.size() > 0)
// 	{
// 		std::lock_guard<std::mutex> lock(commandsMutex);
// 		std::for_each(executionCallback.begin(), executionCallback.end(), [](auto x) { x(); });
// 		executionCallback.clear();
// 	}
// }

// void JRenderer::Flush()
// {
// 	DeviceUtils::Flush(commandQueue, fence, fenceValue, fenceEvent);
// }
