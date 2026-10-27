#pragma once
#include <map>
#include <webgpu/webgpu_cpp.h>

namespace DeviceUtils
{
	struct UniformBuffer;

	inline static const std::string CameraUniformBufferName = "camera";
	inline static const std::string LightUniformBufferName = "lights";
	inline static const std::string ShadowMapUniformBufferName = "shadowMaps";
	inline static const std::string ModelUniformBufferName = "model";
	inline static const std::string MaterialUniformBufferName = "material";
	inline static const std::string AnimationUniformBufferName = "animation";

	using UniformBufferBufferWritter = std::function<size_t(void* data, unsigned int backbufferIndex, size_t offset)>;
	using UniformBufferBufferReader = std::function<void(MaterialVariablesTypes, nlohmann::json& value)>;

	struct UniformBuffer {
		UniformBuffer(size_t bufferSize, bool multiFrame, std::string ubName);
		~UniformBuffer() { Destroy(); }
		
		template<typename T> size_t push(T& data, unsigned int index, size_t offset = 0ULL) {
			size_t offInMap = ((numFrames>0)?index:0)*alignedBufferSize;
			push(&data, offInMap, sizeof(T));
			return offset + sizeof(T);
		}
		void push(void* data, size_t offset, size_t size);
		void Destroy();

		std::string name;
		unsigned int alignedBufferSize;
		wgpu::Buffer uniformBuffer;
		unsigned int numFrames;
		std::vector<std::byte> mappedBuffer;
	};

	std::unique_ptr<UniformBuffer>& GetUniformBuffer(JUUID uuid);

	UniformBufferID CreateUniformBuffer(SceneUnitId id, size_t bufferSize, bool multiFrame, std::string ubName = "");
	void DestroyUniformBuffer(UniformBufferID uuid);
	void DestroyUniformBuffers();
};
