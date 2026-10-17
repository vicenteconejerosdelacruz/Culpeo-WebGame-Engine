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
	};

	std::unique_ptr<IndexBuffer>& GetIndexBuffer(JUUID uuid);
	DEF_TEMPLATE_ID(IndexBuffer, GetIndexBuffer);

	IndexBufferID CreateIndexBuffer(const void* indices, unsigned int indicesCount);
};

using namespace DeviceUtils;
DEF_TEMPLATE_ID_HASH(IndexBuffer);