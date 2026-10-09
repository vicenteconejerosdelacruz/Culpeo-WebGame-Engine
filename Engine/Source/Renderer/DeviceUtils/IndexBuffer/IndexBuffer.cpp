#include "IndexBuffer.h"

namespace DeviceUtils
{
	void InitializeIndexBufferView(wgpu::Device device, const void* indices, unsigned int indicesCount, IndexBufferViewData& ibvData)
	{
		using namespace wgpu;
		BufferDescriptor bufferDesc{};
		bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Index;
    	bufferDesc.size = sizeof(unsigned int) * indicesCount;
		ibvData.indexBuffer = device.CreateBuffer(&bufferDesc);
		ibvData.indexCount = indicesCount;
		device.GetQueue().WriteBuffer(ibvData.indexBuffer, 0, indices, sizeof(unsigned int) * indicesCount);
		//LogCComPtrAddress(std::string("indexBuffer:(" + std::to_string(indicesCount) + ")"), ibvData.indexBuffer);
	}
};