#pragma once
#include <webgpu/webgpu_cpp.h>

namespace DeviceUtils
{
	//vertex buffer
	struct VertexBufferViewData
	{
		wgpu::Buffer vertexBuffer;
		unsigned int vertexCount;
	};

	void InitializeVertexBufferView(wgpu::Device device, const void* vertices, unsigned int vertexSize, unsigned int verticesCount, VertexBufferViewData& vbvData);
};
