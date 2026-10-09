#pragma once
#include <webgpu/webgpu_cpp.h>

namespace DeviceUtils
{
	//index buffer
	struct IndexBufferViewData
	{
		wgpu::Buffer indexBuffer;
		unsigned int indexCount;
	};

	void InitializeIndexBufferView(wgpu::Device device, const void* indices, unsigned int indicesCount, IndexBufferViewData& ibvData);
};
