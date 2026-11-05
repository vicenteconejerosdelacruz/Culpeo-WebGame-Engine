#include "UniformBuffer.h"
#include <Renderer.h>
#include <Scene.h>

extern std::unique_ptr<JRenderer> renderer;

namespace DeviceUtils
{
	static std::map<JUUID, std::unique_ptr<UniformBuffer>> uniformBuffers;

	std::unique_ptr<UniformBuffer>& GetUniformBuffer(JUUID uuid)
	{
		return uniformBuffers.at(uuid);
	}
	
	UniformBufferID CreateUniformBuffer(SceneUnitId id, size_t bufferSize, bool multiFrame, bool storageBuffer, std::string ubName)
	{
		UniformBufferID uniformBufferID = getUUID();
		std::unique_ptr<UniformBuffer> ubData = std::make_unique<UniformBuffer>(bufferSize, multiFrame, ubName);

		assert(!uniformBuffers.contains(uniformBufferID()));
		uniformBuffers.insert_or_assign(uniformBufferID(), std::move(ubData));

		GetSceneUnit(id)->PushToPreRender([uniformBufferID, storageBuffer]
			{
				//create the gpu buffer
				wgpu::BufferDescriptor desc = {};
				desc.nextInChain = nullptr;
				desc.size = uniformBufferID->mappedBuffer.size();
				desc.label = uniformBufferID->name.c_str();
				desc.usage = ((storageBuffer ? wgpu::BufferUsage::Storage : wgpu::BufferUsage::Uniform) | wgpu::BufferUsage::CopyDst);
				desc.mappedAtCreation = false;
				uniformBufferID->uniformBuffer = renderer->GetDevice().CreateBuffer(&desc);
			}
		);

		return uniformBufferID;
	}

	void DestroyUniformBuffer(UniformBufferID uuid)
	{
		// std::lock_guard<std::mutex> lock(uniformBufferMutex);
		uniformBuffers.erase(uuid());
	}

	void DestroyUniformBuffers()
	{
		// std::lock_guard<std::mutex> lock(uniformBufferMutex);
		uniformBuffers.clear();
	}

	UniformBuffer::UniformBuffer(size_t bufferSize, bool multiFrame, std::string ubName)
	{
		name = ubName;
		alignedBufferSize = (bufferSize + 255) & ~255;
		numFrames = multiFrame?JRenderer::numFrames:0U;
		size_t size = alignedBufferSize;
		if(multiFrame) size*=JRenderer::numFrames;
		mappedBuffer.resize(size);
	}

	void UniformBuffer::push(void* data, size_t offset, size_t size)
	{
		memcpy(mappedBuffer.data()+offset, data, size);
		renderer->GetDevice().GetQueue().WriteBuffer(uniformBuffer, offset, mappedBuffer.data() + offset, size);
	}

	void UniformBuffer::Destroy()
	{
		uniformBuffer.Destroy();
		uniformBuffer = nullptr;
	}
}