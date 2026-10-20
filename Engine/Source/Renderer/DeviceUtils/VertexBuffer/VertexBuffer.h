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

	VertexBufferID CreateVertexBuffer(SceneUnitId id,const void* vertices, unsigned int vertexSize, unsigned int verticesCount);
};
