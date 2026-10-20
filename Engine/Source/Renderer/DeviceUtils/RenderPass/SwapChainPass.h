#pragma once
#include <webgpu/webgpu_cpp.h>
//#include <DeviceUtils/RenderToTexture/RenderToTexture.h>
// #include <DeviceUtils/DescriptorHeap/DescriptorHeap.h>
#include <DirectXColors.h>
// #include <functional>

namespace DeviceUtils
{
	struct SwapChainPass
	{
		std::string name;
		unsigned int width;
		unsigned int height;
		// D3D12_VIEWPORT screenViewport;
		// D3D12_RECT scissorRect;
		// DeviceUtils::DescriptorHeap* rtvDescriptorHeap;
		// std::vector<CComPtr<ID3D12Resource>> renderTargets;

		// DXGI_FORMAT depthStencilFormat;
		wgpu::TextureFormat depthStencilFormat;
		// CComPtr<ID3D12DescriptorHeap> depthStencilViewDescriptorHeap;
		// CComPtr<ID3D12Resource> depthStencilTexture;

		void Pass(SceneUnitId unit, std::function<void(SceneUnitId, wgpu::RenderPassEncoder&)> renderCallback, bool clearRTV = true, XMVECTORF32 clearColor = DirectX::Colors::Black);
		wgpu::RenderPassEncoder BeginRenderPass(SceneUnitId unit, wgpu::SurfaceTexture surfaceTexture, wgpu::CommandEncoder& encoder, bool clearRTV = true, XMVECTORF32 clearColor = DirectX::Colors::Black);
		// void CopyFromRenderToTexture(SceneUnitId unit, JUUID RenderToTextureID);
		void EndRenderPass(SceneUnitId unit, wgpu::CommandEncoder& encoder, wgpu::RenderPassEncoder &pass);
		// void ReleaseResources();
		// void Resize(unsigned int width, unsigned int height);
	};

	std::unique_ptr<SwapChainPass>& GetSwapChainPass(JUUID uuid);

	SwapChainPassID CreateSwapChainPass(const std::string name, wgpu::TextureFormat depthStencilFormat);
	void DeleteSwapChainPass(SwapChainPassID swapChainPassId);
};