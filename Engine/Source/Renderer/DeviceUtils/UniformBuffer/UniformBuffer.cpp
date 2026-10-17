#include <map>
// #include <mutex>
#include "UniformBuffer.h"
#include <Renderer.h>

extern std::unique_ptr<JRenderer> renderer;

namespace DeviceUtils
{
	static std::map<JUUID, std::unique_ptr<UniformBuffer>> uniformBuffers;

	std::unique_ptr<UniformBuffer>& GetUniformBuffer(JUUID uuid)
	{
		return uniformBuffers.at(uuid);
	}
	
	// static std::mutex uniformBufferMutex;
	UniformBufferID CreateUniformBuffer(size_t bufferSize, std::string ubName)
	{
		// std::lock_guard<std::mutex> lock(uniformBufferMutex);
		UniformBufferID uniformBufferID = getUUID();
		std::unique_ptr<UniformBuffer> ubData = std::make_unique<UniformBuffer>(bufferSize, ubName);

		/*
		//create the binding layout entry
		wgpu::BindGroupLayoutEntry bindingLayout = {};
		bindingLayout.binding = binding;
		bindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
		bindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
		bindingLayout.buffer.hasDynamicOffset = false;
		bindingLayout.buffer.minBindingSize = bufferSize;

		//create the binding layout descriptor
		wgpu::BindGroupLayoutDescriptor layoutDesc = {};
		layoutDesc.nextInChain = nullptr;
		layoutDesc.label = cbName.c_str();
		layoutDesc.entryCount = 1;
		layoutDesc.entries = &bindingLayout;

		//create the bind group layout
		cbvData->bindGroupLayout = renderer->GetDevice().CreateBindGroupLayout(&layoutDesc);

		//now we create the bind groups
		for(unsigned int i=0U ; i < JRenderer::numFrames; i++ )
		{
			wgpu::BindGroupEntry entry = {};
			entry.binding = 0;
			entry.buffer = cbvData->constantBuffer;
			entry.offset = i * cbvData->alignedConstantBufferSize;
			entry.size = cbvData->alignedConstantBufferSize;

			wgpu::BindGroupDescriptor desc = {};
			desc.layout = cbvData->bindGroupLayout;
			desc.entryCount=1;
			desc.entries = &entry;

			cbvData->bindGroup.push_back(renderer->GetDevice().CreateBindGroup(&desc));
		}
		*/
		assert(!uniformBuffers.contains(uniformBufferID()));
		uniformBuffers.insert_or_assign(uniformBufferID(), std::move(ubData));

		renderer->executionCallback.push_back([uniformBufferID]
			{
				//create the gpu buffer
				wgpu::BufferDescriptor desc = {};
				desc.nextInChain = nullptr;
				desc.size = uniformBufferID->alignedBufferSize * JRenderer::numFrames;
				desc.label = uniformBufferID->name.c_str();
				desc.usage = wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst;
				desc.mappedAtCreation = false;
				uniformBufferID->uniformBuffer = renderer->GetDevice().CreateBuffer(&desc);
			}
		);

		return uniformBufferID;
	}

	void DestroyConstantsBuffer(UniformBufferID uuid)
	{
		// std::lock_guard<std::mutex> lock(uniformBufferMutex);
		uniformBuffers.erase(uuid());
	}

	void DestroyConstantsBuffer()
	{
		// std::lock_guard<std::mutex> lock(uniformBufferMutex);
		uniformBuffers.clear();
	}

	void UniformBuffer::Destroy()
	{
		uniformBuffer.Destroy();
		uniformBuffer = nullptr;
	}
}