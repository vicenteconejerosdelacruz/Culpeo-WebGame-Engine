//#include "pch.h"
#include "Shader.h"
#include <ShaderCompiler.h>
#include <Renderer.h>
#include <Scene.h>
// #include <DeviceUtils/ConstantsBuffer/ConstantsBuffer.h>
// #include <CompilerQueue.h>
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
		};

		const std::set<std::string> uniformsMultiframe = {
			CameraUniformBufferName, LightUniformBufferName, ShadowMapUniformBufferName,
		};

		const std::set<std::string> uniformsFrameOffsets = {
			ModelUniformBufferName, AnimationUniformBufferName,
		};

		auto mapResourceInfo=[&](ShaderResourceInfo& info)
		{
			auto it = std::find_if(uniformsBindingMap.begin(), uniformsBindingMap.end(),
				[&info](auto &bindingMap) { return bindingMap.first==info.variableName; }
			);
			if(it==uniformsBindingMap.end()) 
				return;

			size_t size = uniformsSizesMap.contains(it->first)?uniformsSizesMap.at(it->first):0ULL;
			bool multiFrame = uniformsMultiframe.contains(it->first)?true:false;
			unsigned int numOffsets = uniformsFrameOffsets.contains(it->first)?JRenderer::numFrames:0U;
			it->second = std::make_tuple(info.group, info.binding, size, multiFrame, numOffsets, it->first);
		};

		for(auto &info:shaderInfo)
		{
			mapResourceInfo(info);
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
		std::unordered_map<std::string, ShaderResourceBinding> uniformsBindingMap= {
			{ CameraUniformBufferName, UniformBufferBinding.camera },
			{ LightUniformBufferName, UniformBufferBinding.light },
			{ ShadowMapUniformBufferName, UniformBufferBinding.shadowMaps },
			{ ModelUniformBufferName, UniformBufferBinding.model },
			{ MaterialUniformBufferName, UniformBufferBinding.material },
			{ AnimationUniformBufferName, UniformBufferBinding.animation },
		};

		for(auto it = uniformsBindingMap.begin() ; it != uniformsBindingMap.end(); )
		{
			if(std::get<0>(it->second)==-1) it = uniformsBindingMap.erase(it);
			it++;
		}

		return uniformsBindingMap;
	}

	/*
	void ShaderInstance::CreateResourcesBinding(const ComPtr<ID3D12ShaderReflection>& reflection, const D3D12_SHADER_DESC& desc)
	{
		using namespace DeviceUtils;

		const std::unordered_map<std::string, int& > registersMap =
		{
			{ CameraConstantBufferName, CBV.camera },
			{ LightConstantBufferName, CBV.light },
			{ AnimationConstantBufferName, CBV.animation },
			{ ShadowMapConstantBufferName, CBV.lightsShadowMap },
			{ ShadowMapLightsShaderResourceViewName, SRV.lightsShadowMap },
			{ TextureShaderUsageToString.at(TextureShaderUsage_IBLIrradiance), SRV.iblIrradiance },
			{ TextureShaderUsageToString.at(TextureShaderUsage_IBLPreFilteredEnvironment), SRV.iblPrefiteredEnv },
			{ TextureShaderUsageToString.at(TextureShaderUsage_IBLBRDFLUT), SRV.iblBRDFLUT },
		};

		auto bindSITCBuffer = [this](D3D12_SHADER_INPUT_BIND_DESC& desc, std::string resourceName)
			{
				constantsBuffersParameters.insert_or_assign(resourceName, ShaderConstantsBufferParameter({ .registerId = desc.BindPoint, .numConstantsBuffers = desc.BindCount }));
			};
		auto bindSITUAVRWStructured = [this](D3D12_SHADER_INPUT_BIND_DESC& desc, std::string resourceName)
			{
				uavParameters.insert_or_assign(resourceName, ShaderUAVParameter({ .registerId = desc.BindPoint, .numUAV = desc.BindCount }));
			};
		auto bindSITCSStructured = [this](D3D12_SHADER_INPUT_BIND_DESC& desc, std::string resourceName)
			{
				if (shaderSource.shaderType == JShaderType::COMPUTE_SHADER)
					srvCSParameters.insert_or_assign(resourceName, ShaderSRVParameter({ .registerId = desc.BindPoint, .numSRV = desc.BindCount }));
			};
		auto bindSITPSStructured = [this](D3D12_SHADER_INPUT_BIND_DESC& desc, std::string resourceName)
			{
				if (shaderSource.shaderType == JShaderType::PIXEL_SHADER)
					srvTexParameters.insert_or_assign(StringToTextureShaderUsage.at(resourceName), ShaderSRVParameter({ .registerId = desc.BindPoint, .numSRV = desc.BindCount > 0 ? desc.BindCount : desc.NumSamples })); //if N > 0 -> N else -1
			};
		auto bindSITByteAddress = [this](D3D12_SHADER_INPUT_BIND_DESC& desc, std::string resourceName)
			{
				uavParameters.insert_or_assign(resourceName, ShaderUAVParameter({ .registerId = desc.BindPoint, .numUAV = desc.BindCount }));
			};
		auto bindSITUAVRWByteAddress = [this](D3D12_SHADER_INPUT_BIND_DESC& desc, std::string resourceName)
			{
				uavParameters.insert_or_assign(resourceName, ShaderUAVParameter({ .registerId = desc.BindPoint, .numUAV = desc.BindCount }));
			};
		auto bindSITUAVRWTyped = [this](D3D12_SHADER_INPUT_BIND_DESC& desc, std::string resourceName)
			{
				uavParameters.insert_or_assign(resourceName, ShaderUAVParameter({ .registerId = desc.BindPoint, .numUAV = desc.BindCount }));
			};
		auto bindSITSampler = [this](D3D12_SHADER_INPUT_BIND_DESC& desc, std::string resourceName)
			{
				samplersParameters.insert_or_assign(resourceName, ShaderSamplerParameter({ .registerId = desc.BindPoint, .numSamplers = desc.BindCount }));
			};
		auto bindSITTexture = [this](D3D12_SHADER_INPUT_BIND_DESC& desc, std::string resourceName)
			{
				srvTexParameters.insert_or_assign(StringToTextureShaderUsage.at(resourceName), ShaderSRVParameter({ .registerId = desc.BindPoint, .numSRV = desc.BindCount > 0 ? desc.BindCount : desc.NumSamples })); //if N > 0 -> N else -1
			};

		const std::map<D3D_SHADER_INPUT_TYPE, std::vector<std::function<void(D3D12_SHADER_INPUT_BIND_DESC&, std::string)>>> registerBinding =
		{
			{ D3D_SIT_CBUFFER, { bindSITCBuffer } },
			{ D3D_SIT_UAV_RWSTRUCTURED, { bindSITUAVRWStructured } },
			{ D3D_SIT_STRUCTURED, { bindSITCSStructured, bindSITPSStructured }},
			{ D3D_SIT_BYTEADDRESS, { bindSITByteAddress }},
			{ D3D_SIT_UAV_RWBYTEADDRESS, {bindSITUAVRWByteAddress}},
			{ D3D_SIT_UAV_RWTYPED, { bindSITUAVRWTyped }},
			{ D3D_SIT_SAMPLER, { bindSITSampler }},
			{ D3D_SIT_TEXTURE, { bindSITTexture }},
		};

		for (unsigned int paramIdx = 0; paramIdx < desc.BoundResources; paramIdx++)
		{
			D3D12_SHADER_INPUT_BIND_DESC bindDesc{};
			reflection->GetResourceBindingDesc(paramIdx, &bindDesc);

			std::string resourceName(bindDesc.Name);

			if (registersMap.contains(resourceName)) registersMap.at(resourceName) = bindDesc.BindPoint;
			auto& binders = registerBinding.at(bindDesc.Type);
			for (auto& binder : binders)
			{
				binder(bindDesc, resourceName);
			}
		}
	}
	*/

	/*
	void ShaderInstance::CreateConstantsBuffersVariables(const ComPtr<ID3D12ShaderReflection>& reflection, const D3D12_SHADER_DESC& desc)
	{
		using namespace Templates;
		using namespace DeviceUtils;

		const std::set<std::string> paramsToSkip =
		{
			CameraConstantBufferName,
			LightConstantBufferName,
			AnimationConstantBufferName,
			ShadowMapConstantBufferName,
		};

		for (unsigned int paramIdx = 0; paramIdx < desc.ConstantBuffers; paramIdx++)
		{
			auto cbReflection = reflection->GetConstantBufferByIndex(paramIdx);
			D3D12_SHADER_BUFFER_DESC paramDesc{};
			cbReflection->GetDesc(&paramDesc);

			std::string cbufferName(paramDesc.Name);
			if (HLSLNonConstantsBuffers.contains(cbufferName) || paramsToSkip.contains(cbufferName)) continue;

			bool skipVar = false;
			for (unsigned int varIdx = 0; varIdx < paramDesc.Variables; varIdx++)
			{
				ID3D12ShaderReflectionVariable* varReflection = cbReflection->GetVariableByIndex(varIdx);
				D3D12_SHADER_VARIABLE_DESC varDesc;
				varReflection->GetDesc(&varDesc);

				if (!(varDesc.uFlags & D3D_SVF_USED)) continue;

				ID3D12ShaderReflectionType* varType = varReflection->GetType();
				D3D12_SHADER_TYPE_DESC varTypeDesc;
				varType->GetDesc(&varTypeDesc);

				if (!HLSLVariableClassAllowedTypes.contains(varTypeDesc.Class))
				{
					skipVar = true;
					break;
				}

				std::string varName(varDesc.Name);
				constantsBuffersVariables.insert_or_assign(varName, ShaderConstantsBufferVariable({ .bufferIndex = paramIdx, .size = varDesc.Size, .offset = varDesc.StartOffset }));

				std::string varClass = varTypeDesc.Name;
				bool useNamePattern = false;
				for (auto it = HLSLVariablePatternToMaterialVariableTypes.begin(); it != HLSLVariablePatternToMaterialVariableTypes.end(); it++)
				{
					std::string tieClass = std::get<0>(it->first);
					if (varClass != tieClass) continue;

					std::string tiePattern = std::get<1>(it->first);

					std::string lowerVarName = varName;
					nostd::ToLower(lowerVarName);

					if (lowerVarName.find(tiePattern) == std::string::npos) continue;

					useNamePattern = true;
					break;
				}

				if (useNamePattern) continue;
			}

			if (!skipVar)
			{
				cbufferSize.push_back(paramDesc.Size);
			}
		}
	}
	*/

	/*
	void ShaderInstance::CreateByteCode(const ComPtr<IDxcResult>& result)
	{
		ComPtr<IDxcBlob> code;
		result->GetResult(&code);

		byteCode.resize(code->GetBufferSize());
		memcpy_s(byteCode.data(), code->GetBufferSize(), code->GetBufferPointer(), code->GetBufferSize());
	}
	*/
}