#include "VertexBuffer.h"
#include <Renderer.h>
#include <Scene.h>

extern std::unique_ptr<JRenderer> renderer;

namespace DeviceUtils
{
	static std::map<JUUID, std::unique_ptr<VertexBuffer>> vertexBuffers;

	std::unique_ptr<VertexBuffer>& GetVertexBuffer(JUUID uuid)
	{
		return vertexBuffers.at(uuid);
	}

	VertexBufferID CreateVertexBuffer(SceneUnitId id,const void* vertices, unsigned int vertexSize, unsigned int verticesCount)
	{
		using namespace wgpu;

		VertexBufferID vertexBufferID = getUUID();
		std::unique_ptr<VertexBuffer> vb = std::make_unique<VertexBuffer>();
		vb->vertexCount = verticesCount;

		assert(!vertexBuffers.contains(vertexBufferID()));
		vertexBuffers.insert_or_assign(vertexBufferID(), std::move(vb));

		GetSceneUnit(id)->PushToPreRender([vertexBufferID, vertices, vertexSize, verticesCount]
			{
				BufferDescriptor bufferDesc{};
				bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Vertex;
				bufferDesc.size = vertexSize * verticesCount;
				vertexBufferID->vertexBuffer = renderer->GetDevice().CreateBuffer(&bufferDesc);
				renderer->GetDevice().GetQueue().WriteBuffer(vertexBufferID->vertexBuffer, 0, vertices, vertexSize * verticesCount);
			}
		);

		return vertexBufferID;
	}
}