#include "Material.h"
#include <Renderer.h>
#include "Variables.h"
#include <ShaderCompiler.h>
#include <Scene.h>
// #if defined(_EDITOR)
// #include <Builder/ReleaseBuilder.h>
// #endif

extern std::unique_ptr<JRenderer> renderer;

namespace Templates
{
// #if defined(_EDITOR)

// #include <Editor/JDrawersDef.h>
// #include "MaterialAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDef.h>
// #include "MaterialAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDef.h>
// #include "MaterialAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDef.h>
// #include "MaterialAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDef.h>
// #include "MaterialAtt.h"
// #include <JEnd.h>

// #endif

	MaterialJson::MaterialJson(nlohmann::json& json) : JTemplate(json)
	{
#include <Attributes/JInit.h>
#include "MaterialAtt.h"
#include <JEnd.h>

#include <Attributes/JUpdate.h>
#include "MaterialAtt.h"
#include <JEnd.h>

// #include <Attributes/JV8Att.h>
// #include "MaterialAtt.h"
// #include <JEnd.h>
	}

// #if defined(_EDITOR)
// 	void MaterialJson::WriteJson(nlohmann::json& j)
// 	{
// #include <Editor/JWriteJson.h>
// #include "MaterialAtt.h"
// #include <JEnd.h>
// 	}
// 	void MaterialJson::GatherFiles(std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream)
// 	{
// 		nlohmann::json json = this->json();
// #include <Editor/JReleaseBuilder.h>
// #include "MaterialAtt.h"
// #include <JEnd.h>
// 	}
// #endif

	void MaterialJson::SetPipelineStateCallback(size_t hash, std::function<void()> callback)
	{
		if (pipelineChangeCallbacks.contains(hash)) return;
		pipelineChangeCallbacks.insert_or_assign(hash, callback);
	}

	TEMPDEF_FULL(Material);
	TEMPDEF_REFTRACKER(Material);

	std::unordered_map<MaterialJsonID, std::set<MaterialInstanceID>> materialsTemplatesInstances;

	/*
	void MaterialJsonStep()
	{
		std::set<MaterialJsonID> mats;
		std::transform(Materialtemplates.begin(), Materialtemplates.end(), std::inserter(mats, mats.begin()), [](auto& temps)
			{
				return temps.first;
			}
		);

		std::set<MaterialJsonID> rebuildTextures;
		std::copy_if(mats.begin(), mats.end(), std::inserter(rebuildTextures, rebuildTextures.begin()), [](auto mat)
			{
				return mat->dirty(MaterialJson::Update_textures)
					&& materialsTemplatesInstances.contains(mat)
					&& materialsTemplatesInstances.at(mat).size() > 0ULL;
			}
		);

#if defined(_EDITOR)
		std::unordered_map<TextureJsonID, std::set<std::tuple<TextureShaderUsage, MaterialInstanceID>>> changes;
		std::for_each(rebuildTextures.begin(), rebuildTextures.end(), [&](auto mat)
			{
				for (auto& texUsage : materialTexturesShaderUsage)
				{
					std::string texUsageS = TextureShaderUsageToString.at(texUsage);
					JUUID prevTexUUID = "";
					if (mat->UpdatePrevValues["textures"].contains(texUsageS))
					{
						prevTexUUID = mat->UpdatePrevValues["textures"].at(texUsageS);
					}
					JUUID newTexUUID = "";
					if (mat->at("textures").contains(texUsageS))
					{
						newTexUUID = mat->at("textures").at(texUsageS);
					}
					if (prevTexUUID == newTexUUID) continue;

					for (auto instance : materialsTemplatesInstances.at(mat))
					{
						changes[newTexUUID].insert(std::make_tuple(texUsage, instance));
					}
				}
				mat->clean(MaterialJson::Update_textures);
			}
		);

		if (changes.size() > 0ULL)
		{
			UpdateMaterialTextures(changes);
		}
#endif

		std::set<MaterialJsonID> rebuildPipelineState;
		std::copy_if(mats.begin(), mats.end(), std::inserter(rebuildPipelineState, rebuildPipelineState.begin()), [](auto mat)
			{
				return mat->dirty(MaterialJson::Update_shader_vs) ||
					mat->dirty(MaterialJson::Update_shader_ps) ||
					mat->dirty(MaterialJson::Update_samplers) ||
					mat->dirty(MaterialJson::Update_rasterizerState) ||
					mat->dirty(MaterialJson::Update_blendState) ||
					mat->dirty(MaterialJson::Update_overrideDepthStencil) ||
					mat->dirty(MaterialJson::Update_depthStencil)
					;
			}
		);

		std::for_each(rebuildPipelineState.begin(), rebuildPipelineState.end(), [&](auto mat)
			{
				for (auto& [hash, callback] : mat->pipelineChangeCallbacks)
				{
					callback();
				}
				mat->clean(MaterialJson::Update_shader_vs);
				mat->clean(MaterialJson::Update_shader_ps);
				mat->clean(MaterialJson::Update_samplers);
				mat->clean(MaterialJson::Update_rasterizerState);
				mat->clean(MaterialJson::Update_blendState);
				mat->clean(MaterialJson::Update_overrideDepthStencil);
				mat->clean(MaterialJson::Update_depthStencil);
			}
		);
	}
	*/

	/*
#if defined(_EDITOR)
	static bool updateTexturesProcessorInitialized = false;
	static std::unique_ptr<CommandsProcessor> updateTexturesProcessor;
	void UpdateMaterialTextures(std::unordered_map<TextureJsonID, std::set<std::tuple<TextureShaderUsage, MaterialInstanceID>>> changes)
	{
		using namespace Templates::Texture;

		if (!updateTexturesProcessorInitialized)
		{
			updateTexturesProcessor = std::make_unique<CommandsProcessor>(renderer->d3dDevice, 1, 0x10AD3D);
			updateTexturesProcessorInitialized = true;
		}

		std::thread updateTexThread([](std::unordered_map<TextureJsonID, std::set<std::tuple<TextureShaderUsage, MaterialInstanceID>>> changes)
			{
				updateTexturesProcessor->ResetCommandList();

				std::unordered_map<TextureJsonID, std::set<std::tuple<TextureShaderUsage, MaterialInstanceID>>> postChanges;
				std::transform(changes.begin(), changes.end(), std::inserter(postChanges, postChanges.begin()), [](auto& pair)
					{
						std::pair<TextureJsonID, std::set<std::tuple<TextureShaderUsage, MaterialInstanceID>>> replacement;
						replacement.second = pair.second;
						auto texUUID = pair.first;
						if (!texUUID.empty())
						{
							replacement.first = pair.first;
							return replacement;
						}

						auto usage = std::get<0>(*pair.second.begin());
						switch (usage)
						{
						case TextureShaderUsage_Base:
						{
							replacement.first = GetTextureUUIDByName(defaultBaseTexture);
						}
						break;
						case TextureShaderUsage_NormalMap:
						{
							replacement.first = GetTextureUUIDByName(defaultNormalMap);
						}
						break;
						}
						return replacement;
					}
				);

				for (auto& [texUUID, usageMat] : postChanges)
				{
					if (texUUID.empty()) continue;

					CreateTextureInstance(texUUID(), [&]
						{
							return std::make_unique<TextureInstance>(updateTexturesProcessor->GetCommandList(), texUUID());
						}
					);
				}
				updateTexturesProcessor->CloseCommandList();
				updateTexturesProcessor->RunPostExecution([=]
					{
						for (auto& [texUUID, usageMapSet] : postChanges)
						{
							for (auto [usage, instance] : usageMapSet)
							{
								instance->UpdateTexture(usage, texUUID);
							}
						}
					}
				);
				updateTexturesProcessor->ExecuteCommandList();
			}, changes
		);
		updateTexThread.detach();
	}
#endif
*/
	MaterialInstance::MaterialInstance(
		SceneUnitId id,
		JUUID Instance_uuid,
		JUUID Template_uuid,
		VertexClass vClass,
		bool isShadowed,
		bool hasIBL,
		TextureShaderUsageMap overrideTextures,
		JUUID bindingUUID
	)
	{
		instanceUUID = Instance_uuid;
		materialUUID = Template_uuid;

		materialsTemplatesInstances[materialUUID].insert(instanceUUID);

		MaterialJsonID material = materialUUID;

		auto matTextures = material->textures();
		std::transform(matTextures.begin(), matTextures.end(), std::inserter(textures, textures.end()), [&](auto& pair)
			{
				CreateTextureInstance(pair.second, [&]
					{
						return std::make_unique<TextureInstance>(id, pair.second);
					}
				);
				return TextureUsageInstancePair(pair.first, pair.second);
			}
		);
		std::transform(overrideTextures.begin(), overrideTextures.end(), std::inserter(textures, textures.end()), [&](auto& pair)
			{
				CreateTextureInstance(pair.second, [&]
					{
						return std::make_unique<TextureInstance>(id, pair.second);
					}
				);
				return TextureUsageInstancePair(pair.first, pair.second);
			}
		);
	
		vertexShaderUUID = material->shader_vs();
		pixelShaderUUID = material->shader_ps();

		vertexClass = vClass;
		shadowed = isShadowed;
		ibl = hasIBL;
		CreateMaterialShaderDefines();
		CreateShaderInstances(id);
		CreateSamplers(id);
		LoadVariablesMapping();
	}

	void MaterialInstance::CreateMaterialShaderDefines()
	{
		defines.clear();

		std::vector<std::string> vertexClassDefines = VertexClassDefines.at(vertexClass);

		auto& mat = materialUUID;
		TextureShaderUsageMap texMap = mat->textures();

		if (texMap.size() == 0UL)
		{
			//remove textures components
			std::copy_if(vertexClassDefines.begin(), vertexClassDefines.end(), std::back_inserter(defines), [](auto& def)
				{
					return !VertexTextureComponentsString.contains(def);
				}
			);
		}
		else
		{
			std::move(vertexClassDefines.begin(), vertexClassDefines.end(), std::back_inserter(defines));
			for (auto& [texType, texUUID] : texMap)
			{
				defines.push_back(textureShaderUsageToShaderDefine.at(texType));

				std::unique_ptr<TextureJson>& tex = GetTextureTemplate(texUUID);
				if (NonLinearWgpuFormats.contains(tex->format()))
				{
					std::string srgbTexDefine = textureShaderUsageInGammaSpaceToShaderDefine.at(texType);
					defines.push_back(srgbTexDefine);
				}
			}
		}

		if (shadowed)
		{
			defines.push_back(textureShaderUsageToShaderDefine.at(TextureShaderUsage_ShadowMaps));
		}

		if (ibl)
		{
			defines.push_back(textureShaderUsageToShaderDefine.at(TextureShaderUsage_IBLIrradiance));
			defines.push_back(textureShaderUsageToShaderDefine.at(TextureShaderUsage_IBLPreFilteredEnvironment));
			defines.push_back(textureShaderUsageToShaderDefine.at(TextureShaderUsage_IBLBRDFLUT));
		}
	}

	void MaterialInstance::CreateShaderInstances(SceneUnitId id)
	{
		using namespace ShaderCompiler;
		Source compVS = { .shaderType = VERTEX_SHADER, .shaderTarget = shaderTarget.at(VERTEX_SHADER),.shaderTemplate = vertexShaderUUID, .defines = defines };
		Source compPS = { .shaderType = PIXEL_SHADER, .shaderTarget = shaderTarget.at(PIXEL_SHADER), .shaderTemplate = pixelShaderUUID, .defines = defines };
		vertexShaderInstanceID = vertexShaderUUID() + std::to_string(std::hash<Source>()(compVS));
		pixelShaderInstanceID = pixelShaderUUID() + std::to_string(std::hash<Source>()(compPS));

		CreateShaderInstance(vertexShaderInstanceID(), [this, compVS, id]
			{
				return std::make_unique<ShaderInstance>(id, vertexShaderInstanceID(), compVS.shaderTemplate(), compVS, instanceUUID());
			}
		);
		ShaderIncludesDependencies dependenciesVS;
		Compile(id, vertexShaderInstanceID, compVS, dependenciesVS);
		CreateShaderInstance(pixelShaderInstanceID(), [this, compPS, id]
			{
				return std::make_unique<ShaderInstance>(id, pixelShaderInstanceID(), compPS.shaderTemplate(), compPS, instanceUUID());
			}
		);
		ShaderIncludesDependencies dependenciesPS;
		Compile(id, pixelShaderInstanceID, compPS, dependenciesPS);
	}

	void MaterialInstance::CreateSamplers(SceneUnitId id)
	{
		MaterialJsonID material = materialUUID;
		MaterialInstanceID instance = instanceUUID;

		GetSceneUnit(id)->PushToPreRender([material, instance]
		{
			auto samplersDesc = material->samplers();

			std::unordered_map<std::string, ShaderResourceBinding> psBinding = instance->pixelShaderInstanceID->GetBindingMap();
			std::unordered_map<std::string, ShaderResourceBinding> samplersBinding;
			std::copy_if(psBinding.begin(), psBinding.end(), std::inserter(samplersBinding, samplersBinding.begin()), [](auto& pair)
				{
					return std::get<7>(pair.second);
				}
			);
			unsigned int samplerOffset = samplersBinding.size() > 0 ?
				std::get<1>(std::min_element(samplersBinding.begin(), samplersBinding.end(), [](auto& a, auto& b) {
					return std::get<1>(a.second) < std::get<1>(b.second);
				})->second) : 0;


			std::for_each(samplersBinding.begin(), samplersBinding.end(), [instance, samplerOffset, &samplersDesc](auto& pair)
			{
				unsigned int bindingIndex = std::get<1>(pair.second);
				int index = bindingIndex - samplerOffset;
				MaterialSamplerDesc samplerDesc = samplersDesc.size() > index ? samplersDesc[index] : MaterialSamplerDesc();
				auto sampler = renderer->GetDevice().CreateSampler(&samplerDesc);
				instance->samplers.insert_or_assign(bindingIndex, sampler);
			});
		});
	}

	void MaterialInstance::Destroy()
	{
		// using namespace ShaderCompiler;

		// DeleteShaderInstance(vertexShaderInstanceID());
		// DeleteShaderInstance(pixelShaderInstanceID());
		// for (auto& [type, tex] : textures)
		// {
		// 	DeleteTextureInstance(tex);
		// }
		// materialsTemplatesInstances[materialUUID].erase(instanceUUID);
	}

	void MaterialInstance::LoadVariablesMapping()
	{
		std::map<std::string, std::function<void(MaterialAttributes& att, nlohmann::json j)>> attributeWriter =
		{
			{ "baseColor", [](MaterialAttributes& att, nlohmann::json j){ att.baseColor = ToXMFLOAT3(j); } },
			{ "alphaCut", [](MaterialAttributes& att, nlohmann::json j){ att.alphaCut = j.get<nlohmann::json::number_float_t>(); } },
			{ "colorKey", [](MaterialAttributes& att, nlohmann::json j){ att.colorKey = ToXMFLOAT3(j); } },
			{ "specularExponent", [](MaterialAttributes& att, nlohmann::json j){ att.specularExponent = j.get<nlohmann::json::number_float_t>(); } },
			{ "metallicFactor", [](MaterialAttributes& att, nlohmann::json j){ att.metallicFactor = j.get<nlohmann::json::number_float_t>(); } },
			{ "roughnessFactor", [](MaterialAttributes& att, nlohmann::json j){ att.roughnessFactor = j.get<nlohmann::json::number_float_t>(); } },
			{ "frameIndex", [](MaterialAttributes& att, nlohmann::json j){ att.frameIndex = j.get<nlohmann::json::number_unsigned_t>(); } },
			{ "luminanceCutOff", [](MaterialAttributes& att, nlohmann::json j){ att.luminanceCutOff = j.get<nlohmann::json::number_float_t>(); } },
		};

		auto& material = materialUUID;
		ShaderJsonID shader_ps(material->shader_ps());

		nlohmann::json materialMappedValues = nlohmann::json::array();
		nlohmann::json shaderPSMappedValues = nlohmann::json::array();

		std::map<std::string, nlohmann::json> mappedValues;

		//mapped values comes from the material, vertex and pixel shaders
		if (material->contains("mappedValues")) { materialMappedValues = material->at("mappedValues"); }
		if (shader_ps->contains("mappedValues")) { shaderPSMappedValues = shader_ps->at("mappedValues"); }
		for (nlohmann::json mappedValue : materialMappedValues) { mappedValues.insert_or_assign(mappedValue.at("variable"), mappedValue); }
		for (nlohmann::json mappedValue : shaderPSMappedValues) { mappedValues.insert_or_assign(mappedValue.at("variable"), mappedValue); }

		variablesMapping.clear();

		for (auto& [varName, mappedValue] : mappedValues)
		{
			attributeWriter.at(varName)(materialAttributes,mappedValue.at("value"));
		}
	}

	void MaterialInstance::UpdateTexture(TextureShaderUsage usage, TextureJsonID texture)
	{
		using namespace Templates::Texture;

		if (!textures.at(usage).empty())
			DeleteTextureInstance(textures.at(usage));
		textures.at(usage) = texture();
		// CreateMaterialShaderDefines();
		// CreateShaderInstances();
	}

	UniformBufferID MaterialInstance::CreateUniformBuffers(SceneUnitId id, std::string ubName)
	{
		using namespace DeviceUtils;
		
		std::unordered_map<std::string, ShaderResourceBinding> psBinding = pixelShaderInstanceID->GetBindingMap();
				
		auto itPS = std::find_if(psBinding.begin(), psBinding.end(), [ubName](auto& pair) { return pair.first == ubName; });
		
		if(itPS!=psBinding.end())
		{
			auto [group, binding, buffer_size, multi_frame, storage_buffer, name, is_texture, is_sampler] = itPS->second;
			if(is_texture || is_sampler)
			{
				std::cerr << "Error: " << ubName << " is not a uniform buffer in pixel shader instance " << pixelShaderInstanceID() << std::endl;
				return UniformBufferID("");
			}
			return CreateUniformBuffer(id, buffer_size, multi_frame, storage_buffer, ubName);
		}

		return UniformBufferID("");
	}

	std::vector<std::vector<ShaderResourceBinding>> MaterialInstance::GetShaderBindings()
	{
		auto bindings = pixelShaderInstanceID->GetBindingMap();
		auto maxGroups = std::get<0>(std::max_element(bindings.begin(),bindings.end(),[](auto a, auto b){
			return std::get<0>(a.second) < std::get<0>(b.second);
		})->second);

		std::vector<std::vector<ShaderResourceBinding>> shaderBindings;
		for(unsigned int group=0; group <= maxGroups; group++)
		{
			shaderBindings.push_back({});
			std::unordered_map<std::string, ShaderResourceBinding> samegroup;
			std::copy_if(bindings.begin(), bindings.end(), std::inserter(samegroup,samegroup.begin()), [group](auto binding){ return std::get<0>(binding.second)==group;});
			std::vector<ShaderResourceBinding> sorted;
			std::transform(samegroup.begin(), samegroup.end(), std::back_inserter(sorted), [](auto g){ return g.second; });
			std::sort(sorted.begin(), sorted.end(), [](auto a, auto b){ return std::get<1>(a) < std::get<1>(b);});
			shaderBindings.back()=sorted;
		}
		return shaderBindings;
	}

	void DestroyMaterialInstance(JUUID materialInstance)
	{
		DeleteMaterialInstance(materialInstance);
	}
}
