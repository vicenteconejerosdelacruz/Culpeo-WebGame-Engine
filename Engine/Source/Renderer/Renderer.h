#pragma once

#include <numbers>
#include <GLFW/glfw3.h>
#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_glfw.h>
// #include <dxgi1_6.h>
#include <RenderPass/RenderPass.h>
// using namespace Microsoft::WRL;
// using namespace DirectX;

struct JRenderer
{
	~JRenderer() {}
	// static const constexpr unsigned int numFrames = 3;
	static const constexpr float fovAngleY = (70.0f * std::numbers::pi_v<float> / 180.0f);
	// static inline const std::unordered_map<DXGI_FORMAT, DXGI_FORMAT> depthFallback = {
	// 	{ DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT }
	// };
	// static inline const std::unordered_map<DXGI_FORMAT, DXGI_FORMAT> depthFormatTexConversion = {
	// 	{ DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_R32_TYPELESS },
	// 	{ DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_R24G8_TYPELESS }
	// };
	// static inline const std::unordered_map<DXGI_FORMAT, DXGI_FORMAT> depthFormatSRVConversion = {
	// 	{ DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_R32_FLOAT },
	// 	{ DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_R24_UNORM_X8_TYPELESS }
	// };

	//CREATE
	// void Initialize(HWND hwnd);
	void Initialize();
	// void CreateSwapChainPass();
	void CreateWindow();
	void ResizeSwapChain(int width, int height);
	void CreateSurface();
	void ConfigureSurface(uint32_t width, uint32_t height);

	//READ&GET
	// unsigned int GetBackBufferIndex();
	wgpu::Instance GetInstance() { return instance; }
	wgpu::Adapter GetAdapter() { return adapter; }
	wgpu::Device GetDevice() { return device; }
	GLFWwindow* GetWindow() { return window; }

	//UPDATE
	// void UpdateViewportPerspective();
	// void Resize(unsigned int width, unsigned int height);
	// void ExecuteCommands(CComPtr<ID3D12GraphicsCommandList2>& commandList, std::function<void()> callback = nullptr);
	// void Present();
	// void Flush();

	wgpu::Instance instance;
	wgpu::Adapter adapter;
	wgpu::Device device;
	GLFWwindow* window = nullptr;
	int windowWidth;
	int windowHeight;
	wgpu::Surface surface;
	wgpu::TextureFormat format;

	//Depth 32F support
	// bool d32FSupported;

	//app window reference
	// HWND hwnd;

	//d3d device resources in order of creation
	// CComPtr<ID3D12Device2> d3dDevice;
	// CComPtr<ID3D12CommandQueue> commandQueue;
	// CComPtr<IDXGISwapChain4> swapChain;
	// DXGI_FORMAT swapChainFormat;
	// bool supportsTearing;

	//GPU <-> CPU synchronization 
	//unsigned int backBufferIndex;
	// CComPtr<ID3D12Fence> fence;
	// unsigned long long fenceValue = 0;
	// unsigned long long frameFenceValues[numFrames] = {};
	// HANDLE fenceEvent;
	// std::vector<std::function<void()>> executionCallback;

	//window based values
	// D3D12_VIEWPORT screenViewport;
	// D3D12_RECT scissorRect;

	//the swap chain pass
	// RenderPassInstanceID swapChainPass;
};
