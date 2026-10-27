#pragma once
#include <map>
#include <webgpu/webgpu_cpp.h>

namespace DeviceUtils
{
	//index buffer
	struct IndexBuffer
	{
		wgpu::Buffer indexBuffer;
		unsigned int indexCount;
		std::vector<std::byte> rawData;
	};

	std::unique_ptr<IndexBuffer>& GetIndexBuffer(JUUID uuid);

	IndexBufferID CreateIndexBuffer(SceneUnitId id,const void* indices, unsigned int indicesCount);
};