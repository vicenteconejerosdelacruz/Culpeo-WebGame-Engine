#pragma once
#include <map>
#include <webgpu/webgpu_cpp.h>

namespace DeviceUtils
{
	//vertex buffer
	struct VertexBuffer
	{
		wgpu::Buffer vertexBuffer;
		unsigned int vertexCount;
	};

	std::unique_ptr<VertexBuffer>& GetVertexBuffer(JUUID uuid);
	DEF_TEMPLATE_ID(VertexBuffer, GetVertexBuffer);

	VertexBufferID CreateVertexBuffer(const void* vertices, unsigned int vertexSize, unsigned int verticesCount);
};

using namespace DeviceUtils;
DEF_TEMPLATE_ID_HASH(VertexBuffer);