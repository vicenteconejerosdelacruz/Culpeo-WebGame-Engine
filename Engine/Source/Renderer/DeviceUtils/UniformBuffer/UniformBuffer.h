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
		UniformBuffer(size_t size, std::string name);
		~UniformBuffer() { Destroy(); }
		
		template<typename T> size_t push(T& data, unsigned int index, size_t offset = 0ULL) {
			push(&data, alignedBufferSize * index + offset, sizeof(T));
			return offset + sizeof(T);
		}
		void push(void* data, size_t offset, size_t size);
		void Destroy();

		std::string name;
		unsigned int alignedBufferSize;
		wgpu::Buffer uniformBuffer;
		std::vector<std::byte> mappedBuffer;
	};

	std::unique_ptr<UniformBuffer>& GetUniformBuffer(JUUID uuid);

	UniformBufferID CreateUniformBuffer(SceneUnitId id, size_t bufferSize, std::string ubName = "");
	void DestroyUniformBuffer(UniformBufferID uuid);
	void DestroyUniformBuffers();
};
