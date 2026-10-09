#include "VertexBuffer.h"

namespace DeviceUtils
{
	void InitializeVertexBufferView(wgpu::Device device, const void* vertices, unsigned int vertexSize, unsigned int verticesCount, VertexBufferViewData& vbvData)
	{
		using namespace wgpu;
		BufferDescriptor bufferDesc{};
		bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Vertex;
    	bufferDesc.size = vertexSize * verticesCount;
		vbvData.vertexBuffer = device.CreateBuffer(&bufferDesc);
		vbvData.vertexCount = verticesCount;
		device.GetQueue().WriteBuffer(vbvData.vertexBuffer, 0, vertices, vertexSize * verticesCount);
		//LogCComPtrAddress(std::string("vertexBuffer:(" + std::to_string(vertexSize) + "," + std::to_string(verticesCount) + ")"), vbvData.vertexBuffer);
	}
}