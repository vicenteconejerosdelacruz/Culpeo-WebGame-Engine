//#include "pch.h"
#include "Shader.h"
#include <ShaderCompiler.h>
#include <Renderer.h>
#include <Scene.h>
#include <Light.h>
#include <Material/Material.h>
#include <Textures/Texture.h>

#if defined(_EDITOR)
//#include <Builder/ReleaseBuilder.h>
#endif

extern std::unique_ptr<JRenderer> renderer;

namespace Templates {

#if defined(_EDITOR)

// #include <Editor/JDrawersDef.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDef.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDef.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDef.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDef.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

#endif

	namespace Shader
	{
		ShaderIncludesDependencies dependencies;
		std::set<Source> GetSourcesFromShaderUUID(JUUID shaderTemplate)
		{
			ShaderIncludesDependencies matchingUUIDs;
			std::copy_if(dependencies.begin(), dependencies.end(), std::inserter(matchingUUIDs, matchingUUIDs.begin()),
				[shaderTemplate](const auto& dep) { return dep.first.shaderTemplate() == shaderTemplate; }
			);
			std::set<Source> sources;
			std::transform(matchingUUIDs.begin(), matchingUUIDs.end(), std::inserter(sources, sources.begin()), [](const auto& pair) { return pair.first; });
			return sources;
		}
		std::multimap<std::string, std::string> fileNameToShaderTemplate;
	};

	ShaderJson::ShaderJson(nlohmann::json& json) : JTemplate(json)
	{
#include <Attributes/JInit.h>
#include "ShaderAtt.h"
#include <JEnd.h>

#include <Attributes/JUpdate.h>
#include "ShaderAtt.h"
#include <JEnd.h>

// #include <Attributes/JV8Att.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

		Shader::fileNameToShaderTemplate.insert({ path(),uuid() });
	}

#if defined(_EDITOR)
	void ShaderJson::WriteJson(nlohmann::json& j)
	{
// #include <Editor/JWriteJson.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>
	}
// 	void ShaderJson::GatherFiles(std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream)
// 	{
// 		nlohmann::json json = this->json();
// #include <Editor/JReleaseBuilder.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>
// 	}
#endif

	TEMPDEF_FULL(Shader);
	TEMPDEF_REFTRACKER(Shader);

	ShaderInstance::ShaderInstance(
		SceneUnitId id,
		JUUID instance_uuid,
		JUUID uuid, Source params,
		JUUID bindingUUID
	)
	{
		using namespace ShaderCompiler;
		using namespace Templates::Shader;

		instanceUUID = instance_uuid;
		shaderUUID = uuid;

		// Compile(id, instanceUUID, params, dependencies);
	}

	void ShaderInstance::CreateVSSemantics()
	{
		using namespace ShaderCompiler;
		std::vector<VertexAttributeInfo> vertexAttrs = ParseVertexInputAttributes(byteCode);
		std::transform(vertexAttrs.begin(),vertexAttrs.end(), std::back_inserter(vsSemantics),[](auto& info){ return info.name; });
	}

	void ShaderInstance:: CreateResourcesBinding()
	{
		using namespace ShaderCompiler;
		using namespace DeviceUtils;
		using namespace Templates::Texture;
		using namespace Templates::Material;
		using namespace Animation;

		shaderInfo = ParseWGSLBindings(byteCode);

		const std::unordered_map<std::string, ShaderResourceBinding&> uniformsBindingMap =
		{
			{ CameraUniformBufferName, UniformBufferBinding.camera },
			{ LightUniformBufferName, UniformBufferBinding.light },
			{ ShadowMapUniformBufferName, UniformBufferBinding.shadowMaps },
			{ ModelUniformBufferName, UniformBufferBinding.model },
			{ MaterialUniformBufferName, UniformBufferBinding.material },
			{ AnimationUniformBufferName, UniformBufferBinding.animation },
		};

		const std::unordered_map<std::string, size_t> uniformsSizesMap = {
			{ CameraUniformBufferName, sizeof(CameraAttributes) },
			{ ModelUniformBufferName, sizeof(XMMATRIX) },
			{ MaterialUniformBufferName, sizeof(MaterialAttributes) },
			{ LightUniformBufferName, sizeof(LightPool) },
			{ AnimationUniformBufferName, sizeof(BonesMatrices) },
		};

		const std::set<std::string> uniformsMultiframe = {
			CameraUniformBufferName, LightUniformBufferName, ShadowMapUniformBufferName, ModelUniformBufferName, AnimationUniformBufferName
		};

		const std::unordered_map<std::string, ShaderResourceBinding&> texturesBindingMap =
		{
			{ baseTexture, TextureBinding.baseTexture },
			{ normalMap, TextureBinding.normalMap },
			{ roughnessMap, TextureBinding.roughnessMap },
			{ metallicMap, TextureBinding.metallicMap },
			{ aoMap, TextureBinding.aoMap },
			{ emissiveMap, TextureBinding.emissiveMap },
			{ lightsShadowMap, TextureBinding.lightsShadowMap },
			{ iblIrradiance, TextureBinding.iblIrradiance },
			{ iblPrefiteredEnv, TextureBinding.iblPrefiteredEnv },
			{ iblBRDFLUT, TextureBinding.iblBRDFLUT }
		};

		const std::unordered_map<std::string, ShaderResourceBinding&> samplersBindingMap =
		{
			{ Sampler0Name, SamplerBinding.baseTextureSampler }
		};

		const std::unordered_map<std::string, ShaderResourceBinding&> storageBindingMap =
		{
			{ LightUniformBufferName, UniformBufferBinding.light },
			{ AnimationUniformBufferName, UniformBufferBinding.animation },
		};

		auto mapUniformBufferResourceInfo=[&](ShaderResourceInfo& info)
		{
			if(info.isSampler || info.isTexture)
				return;

			auto it = std::find_if(uniformsBindingMap.begin(), uniformsBindingMap.end(),
				[&info](auto &bindingMap) { return bindingMap.first==info.variableName; }
			);
			if(it==uniformsBindingMap.end()) 
				return;

			size_t size = uniformsSizesMap.contains(it->first)?uniformsSizesMap.at(it->first):0ULL;
			bool multiFrame = uniformsMultiframe.contains(it->first)?true:false;
			bool storageBuffer = storageBindingMap.contains(it->first)?true:false;
			it->second = std::make_tuple(info.group, info.binding, size, multiFrame, storageBuffer, it->first, false, false);
		};

		auto mapSamplersResourceInfo=[&](ShaderResourceInfo& info)
		{
			if(!info.isSampler)
				return;

			auto it = std::find_if(samplersBindingMap.begin(), samplersBindingMap.end(),
				[&info](auto &bindingMap) { return bindingMap.first==info.variableName; });
			if(it==samplersBindingMap.end())
				return;

			it->second = std::make_tuple(info.group, info.binding, 0ULL, false, false, it->first, false, true);
		};

		auto mapTexturesResourceInfo=[&](ShaderResourceInfo& info)
		{
			if(!info.isTexture)
				return;

			auto it = std::find_if(texturesBindingMap.begin(), texturesBindingMap.end(),
				[&info](auto &bindingMap) { return bindingMap.first==info.variableName; });
			if(it==texturesBindingMap.end())
				return;

			it->second = std::make_tuple(info.group, info.binding, 0ULL, false, false, it->first, true, false);
		};

		for(auto &info:shaderInfo)
		{
			mapUniformBufferResourceInfo(info);
			mapSamplersResourceInfo(info);
			mapTexturesResourceInfo(info);
		}
	}

	void ShaderInstance::CreateShaderModule(SceneUnitId id)
	{
		GetSceneUnit(id)->PushToPreRender([&] {

			WGPUShaderSourceWGSL wgslDesc{};
			wgslDesc.chain.sType = static_cast<WGPUSType>(WGPUSType_ShaderSourceWGSL);
			wgslDesc.chain.next = nullptr;
			wgslDesc.code = { byteCode.c_str() , WGPU_STRLEN };

			wgpu::ShaderModuleDescriptor descriptor{};
			descriptor.label = instanceUUID.c_str();
			descriptor.nextInChain = reinterpret_cast<const wgpu::ChainedStruct*>(&wgslDesc);

			// 4. Crear el módulo en la GPU
			shaderModule = renderer->GetDevice().CreateShaderModule(&descriptor);
			if (!shaderModule) {
				std::cerr << "[WebGPU] Error al compilar el shader WGSL." << std::endl;
			} else 
			{
				std::cout << "[WebGPU] compilacion completa." << std::endl;
			}
		});
	}

	std::unordered_map<std::string, ShaderResourceBinding> ShaderInstance::GetBindingMap()
	{
		using namespace Templates::Texture;
		using namespace Templates::Material;

		std::unordered_map<std::string, ShaderResourceBinding> uniformsBindingMap= {
			{ CameraUniformBufferName, UniformBufferBinding.camera },
			{ LightUniformBufferName, UniformBufferBinding.light },
			{ ShadowMapUniformBufferName, UniformBufferBinding.shadowMaps },
			{ ModelUniformBufferName, UniformBufferBinding.model },
			{ MaterialUniformBufferName, UniformBufferBinding.material },
			{ AnimationUniformBufferName, UniformBufferBinding.animation },
			{ Sampler0Name, SamplerBinding.baseTextureSampler },
			{ baseTexture, TextureBinding.baseTexture },
			{ normalMap, TextureBinding.normalMap },
			{ roughnessMap, TextureBinding.roughnessMap },
			{ metallicMap, TextureBinding.metallicMap },
			{ aoMap, TextureBinding.aoMap },
			{ emissiveMap, TextureBinding.emissiveMap },
			{ lightsShadowMap, TextureBinding.lightsShadowMap },
			{ iblIrradiance, TextureBinding.iblIrradiance },
			{ iblPrefiteredEnv, TextureBinding.iblPrefiteredEnv },
			{ iblBRDFLUT, TextureBinding.iblBRDFLUT },
		};

		for(auto it = uniformsBindingMap.begin() ; it != uniformsBindingMap.end(); )
		{
			if(std::get<0>(it->second)==-1) { it = uniformsBindingMap.erase(it); continue;}
			it++;
		}

		return uniformsBindingMap;
	}
}