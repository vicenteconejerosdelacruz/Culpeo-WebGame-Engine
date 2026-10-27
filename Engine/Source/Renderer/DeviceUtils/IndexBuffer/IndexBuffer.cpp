#include "IndexBuffer.h"
#include <Renderer.h>
#include <Scene.h>

extern std::unique_ptr<JRenderer> renderer;

namespace DeviceUtils
{
	static std::map<JUUID, std::unique_ptr<IndexBuffer>> indexBuffer;

	std::unique_ptr<IndexBuffer>& GetIndexBuffer(JUUID uuid)
	{
		return indexBuffer.at(uuid);
	}

	IndexBufferID CreateIndexBuffer(SceneUnitId id,const void* indices, unsigned int indicesCount)
	{
		using namespace wgpu;

		IndexBufferID indexBufferID = getUUID();
		std::unique_ptr<IndexBuffer> ib = std::make_unique<IndexBuffer>();
		ib->indexCount = indicesCount;

		size_t totalSize = sizeof(unsigned int) * indicesCount;
		ib->rawData.resize(totalSize);
		std::memcpy(ib->rawData.data(), indices, totalSize);

		assert(!indexBuffer.contains(indexBufferID()));
		indexBuffer.insert_or_assign(indexBufferID(), std::move(ib));

		GetSceneUnit(id)->PushToPreRender([indexBufferID]
			{
				BufferDescriptor bufferDesc{};
				bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Index;
				bufferDesc.size = indexBufferID->rawData.size();
				indexBufferID->indexBuffer = renderer->GetDevice().CreateBuffer(&bufferDesc);
				renderer->GetDevice().GetQueue().WriteBuffer(
					indexBufferID->indexBuffer,
					0,
					indexBufferID->rawData.data(), 
					indexBufferID->rawData.size()
				);
			}
		);

		return indexBufferID;
	}
};