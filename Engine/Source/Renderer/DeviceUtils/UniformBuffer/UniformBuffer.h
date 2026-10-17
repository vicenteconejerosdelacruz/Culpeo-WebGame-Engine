#pragma once
#include <map>
#include <webgpu/webgpu_cpp.h>

namespace DeviceUtils
{
	struct UniformBuffer;

	inline static const std::string CameraUniformBufferName = "camera";
	inline static const std::string LightUniformBufferName = "lights";
	inline static const std::string ShadowMapUniformBufferName = "shadowMaps";
	inline static const std::string AnimationUniformBufferName = "animation";

	using UniformBufferBufferWritter = std::function<size_t(void* data, unsigned int backbufferIndex, size_t offset, size_t slot)>;
	using UniformBufferBufferReader = std::function<void(MaterialVariablesTypes, nlohmann::json& value)>;

	struct UniformBuffer {
		UniformBuffer(size_t size, std::string name) : alignedBufferSize((size + 255) & ~255), name(name) { mappedUniformBuffer.resize(alignedBufferSize); }
		~UniformBuffer() { Destroy(); }
		
		template<typename T> size_t push(T& data, unsigned int index, size_t offset = 0ULL) {
			memcpy(mappedUniformBuffer.data() + alignedBufferSize * index + offset, &data, sizeof(T));
			return offset + sizeof(T);
		}
		
		void Destroy();

		std::string name;
		unsigned int alignedBufferSize;
		std::vector<std::byte> mappedUniformBuffer;
		wgpu::Buffer uniformBuffer;
		// wgpu::BindGroupLayout bindGroupLayout;
		// std::vector<wgpu::BindGroup> bindGroup;
	};

	std::unique_ptr<UniformBuffer>& GetUniformBuffer(JUUID uuid);
	DEF_TEMPLATE_ID(UniformBuffer, GetUniformBuffer);

	UniformBufferID CreateUniformBuffer(size_t bufferSize, std::string ubName = "");
	void DestroyUniformBuffer(UniformBufferID uuid);
	void DestroyUniformBuffer();
};

using namespace DeviceUtils;
DEF_TEMPLATE_ID_HASH(UniformBuffer);