#include "Renderable.h"
#include <SceneObject.h>
#include <Scene.h>
#include <Renderer.h>
#include <Textures/Texture.h>
// #include <DeviceUtils/RootSignature/RootSignature.h>
// #include <DeviceUtils/PipelineState/PipelineState.h>
// #include <Renderable/RenderableBoundingBox.h>
// #include <NoMath.h>
// #if defined(_EDITOR)
// #include <Builder/ReleaseBuilder.h>
// #endif

extern std::unique_ptr<JRenderer> renderer;

// #if defined(_EDITOR)
// namespace Editor
// {
// 	extern void SelectRenderable(RenderableID renderable);
// 	extern bool IsPlaying(SceneUnitId unit);
// 	extern bool IsPaused(SceneUnitId unit);
// 	extern std::unordered_map<SceneUnitId, CameraID> levelCameraUUID;
// 	extern std::unordered_map<SceneUnitId, CameraID> editorCameraUUID;
// };
// #endif

namespace Scene
{
	// SODEF_FULL(Renderable);

#include <TrackUUID/JDef.h>
#include "RenderableAtt.h"
#include <JEnd.h>

// #if defined(_EDITOR)

// #include <Editor/JDrawersDef.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDef.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDef.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDef.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDef.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #endif

	// std::unordered_map<RenderableID, SequencePlayer*> animationPlayers;

// #if defined(_EDITOR)

// 	void Renderable::GatherFiles(nlohmann::json& json, std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream)
// 	{
// #include <Editor/JReleaseBuilder.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>
// 	}
// #endif

	Renderable::Renderable(nlohmann::json& json) :Controller(json)
	{
#include <Attributes/JInit.h>
#include "RenderableAtt.h"
#include <JEnd.h>

#include <Attributes/JUpdate.h>
#include "RenderableAtt.h"
#include <JEnd.h>

// #include <Attributes/JV8Att.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

		// lifecycleState->store(false);
		// //RENAME_ON_DELETION(Renderable);
		// animationStepLock = std::make_unique<std::atomic_bool>(false);
		// for (unsigned int i = 0; i < JRenderer::numFrames; i++)
		// {
		// 	constantsBuffersLock[i] = std::make_unique<std::atomic_bool>(false);
		// }
		// markedForDelete.Hook([&]
		// 	{
		// 		lifecycleState->wait(false);
		// 		RenameRenderableSceneObject(SUuuid(), "delete-" + uuid());
		// 		if (at("physicObject").is_array() && at("physicObject").size() > 0)
		// 		{
		// 			for (unsigned int i = 0; i < at("physicObject").size(); i++)
		// 			{
		// 				auto& phO = GetPhysicObject(at("physicObject").at(i));
		// 				if (phO->actor && phO->actor->userData)
		// 					phO->actor->userData = nullptr;
		// 			}
		// 		}
		// 	}
		// );
	}

    void Renderable::Map(SUUUID so)
    {
		Controller::Map(so);

#include <TrackUUID/JInsert.h>
#include "RenderableAtt.h"
#include <JEnd.h>

		worldUb = CreateUniformBuffer(unit, sizeof(XMMATRIX), true, false,"world");
		CreateMeshInstances();
    }

    void Renderable::Unmap()
    {
#include <TrackUUID/JErase.h>
#include "RenderableAtt.h"
#include <JEnd.h>
		Controller::Unmap();
    }

#if defined(_EDITOR)
	void Renderable::WriteJson(nlohmann::json& j)
	{
		#include <Editor/JWriteJson.h>
		#include "RenderableAtt.h"
		#include <JEnd.h>
		Controller::WriteJson(j);
	}
#endif

	/*
	void Renderable::Initialize()
	{
		using namespace Animation;
		using namespace ComputeShader;

		CreateMeshInstances(); //why here, this is a special case, as Animables depends of animables which is created in this function

#include <TrackUUID/JInsert.h>
#include "RenderableAtt.h"
#include <JEnd.h>

		if (!animable.empty())
		{
			AttachAnimation(SUuuid(), model3D->animations);
			//boundingBoxCompute = CreateRenderableBoundingBox(MAKESUUUID(unit, uuid()));
			sequencePlayer.renderable = SUuuid();
			CreateAnimationThread();
		}

		SetInitialConditions();

#if defined(_EDITOR)
		OnPick = [&] { Editor::SelectRenderable(SUuuid()); };
#endif
		SceneObject::Initialize();
	}
	*/

	/*
	void Renderable::SetInitialConditions()
	{
		lastAnimationTime = 0.0f;
		forceAnimation = true;
		animationTransformation = XMMatrixIdentity();

		if (!animable.empty())
		{
			animationTime(0.0f);
			SetCurrentAnimation(animationSequence(), animationTime(), animationTimeFactor(), animationPlay(), animationLoop());

			// StepAnimation ya gestiona la espera y la señalización correctamente
			StepAnimation(0.0f);
		}

		updateRotationQ();
		deleteFrames = JRenderer::numFrames;
	}
	*/

	void Renderable::BindToScene()
	{
#include <TrackUUID/JInsert.h>
#include "RenderableAtt.h"
#include <JEnd.h>

		BindCameras();
		BindShadowMapCameras();
		// lifecycleState->store(true);
		// lifecycleState->notify_all();
	}

	void Renderable::Bind(JUUID uuid)
	{
		if (uuid.empty())
			return;

		/*
		switch (GetSceneObjectType(unit, uuid))
		{
		case SO_Cameras:
		{
			bindedCameras.insert(MAKESUUUID(unit, uuid));
		}
		break;
		}
		*/
	}

	void Renderable::BindToCamera(JUUID camId)
	{
		CameraID cam(camId);
		bindedCameras.insert(cam);
		cam->BindRenderable(uuid());
	}

	void Renderable::BindCameras()
	{
		auto cams = cameras();
		for (auto& suuid : cams)
		{
			SceneObjectID cam(unit,suuid);
			JUUID uuid = cam->controllers_get("camera");
			if (uuid.empty() || bindedCameras.contains(uuid))
				continue;
			BindCamera(uuid);
		}
	}

	void Renderable::BindCamera(JUUID cuuid)
	{
		Scene::BindToScene(unit, uuid(), cuuid);
	}

	void Renderable::BindShadowMapCameras()
	{
		if (!castShadows()) return;

		/*
		auto lights = GetShadowMapLights(unit);
		for (auto uuid : lights)
		{
			LightID light = MAKESUUUID(unit, uuid);
			for (CameraID cam : light->shadowMapCameras)
			{
				BindCamera(cam.uuid());
			}
		}
		*/
	}

	void Renderable::UnbindFromScene()
	{
		// using namespace ComputeShader;

#include <TrackUUID/JErase.h>
#include "RenderableAtt.h"
#include <JEnd.h>

		UnbindCameras();
		Scene::UnbindFromScene(unit, uuid());
		// //if (!boundingBoxCompute.empty())
		// //{
		// //	DeleteRenderableBoundingBox(boundingBoxCompute());
		// //	boundingBoxCompute.clear();
		// //}
		// if (animationPlayers.contains(MAKESUUUID(unit, uuid())))
		// {
		// 	animationPlayers.erase(MAKESUUUID(unit, uuid()));
		// }
	}

	void Renderable::Unbind(JUUID uuid)
	{
		bindedCameras.erase(uuid);
	}

	void Renderable::UnbindCameras()
	{
		auto cams = cameras();
		for (auto& uuid : cams)
		{
			if (!bindedCameras.contains(uuid))
				continue;
			UnbindCamera(uuid);
		}

		// if (castShadows())
		// {
		// 	auto smlights = GetShadowMapLights(unit);
		// 	for (auto& uuid : smlights)
		// 	{
		// 		LightID l = MAKESUUUID(unit, uuid);
		// 		l->UnbindRenderableFromShadowMapCamera(SUuuid());
		// 	}
		// }
	}

	void Renderable::UnbindCamera(JUUID cuuid)
	{
		CameraID cam(cuuid);
		cam->UnbindRenderable(uuid());
		Scene::UnbindFromScene(unit, uuid(), cuuid);
	}

/*
#if defined(_EDITOR)
	void Renderable::DropJsonMoldAttributes(nlohmann::json& j)
	{
		SceneObject::DropJsonMoldAttributes(j);
		j.at("cameras") = nlohmann::json::array({});
	}
#endif
*/

	XMMATRIX Renderable::world()
	{
		auto& so = *sceneObject;
		
		XMVECTOR posV = so->positionV();
		XMVECTOR scaleV = so->scaleV();

		XMMATRIX rotationM = XMMatrixRotationQuaternion(so->rotationQuaternion);
		XMMATRIX scaleM = XMMatrixScalingFromVector(scaleV);
		XMMATRIX positionM = XMMatrixTranslationFromVector(posV);

		XMMATRIX worldM = XMMatrixMultiply(XMMatrixMultiply(scaleM, rotationM), positionM);

		/*
		XMMATRIX animT = (!animationUseTransformation()) ? worldM : XMMatrixMultiply(animationTransformation, worldM);
		if (attachedTo().empty())
		{
			return animT;
		}
		if (attachedBone().empty())
		{
			return XMMatrixMultiply(animT, RenderableID(unit, attachedTo())->world());
		}
		else
		{
			RenderableID parent(unit, attachedTo());
			auto [mm, pos, a, b, c] = parent->GetBoneTransformation(attachedBone());
			return XMMatrixMultiply(animT, mm);
		}
		*/
		return worldM;
	}

	void Renderable::CreateMeshInstances()
	{
		/*
		using namespace Templates;

		if (Model3DTemplateExist(model()))
		{
			CreateModel3DInstance(model(), [this]
				{
					return std::make_unique<Model3DInstance>(unit, model(), uuid());
				}
			);
			model3D = model();
			meshes = model3D->meshes;
			if (model3D->animations)
			{
#if defined(_EDITOR)
				Model3DJsonID model3dJson = model();
				model3dJson->ListenUpdate(Model3DJson::Update_animationSequences, SUuuid(), [&]()
					{
						RebuildAnimationSequences();
					}
				);
#endif
				animable = model3D;
			}

			if (!animable)
			{
				CreateBoundingBox();
			}
			else
			{
				CreateAnimationSequences();
			}
		}
		else*/
		 if (!meshMaterial().mesh.empty() && meshMaterial().mesh.contains("primitive") && !meshMaterial().mesh.at("primitive").empty() && !meshMaterial().materialUUID.empty())
		{
			nlohmann::json mesh = meshMaterial().mesh;
			meshes.push_back(GetMeshInstance(unit, mesh)->uuid);
			CreateBoundingBox();
		}
	}

	std::vector<RenderPassInstanceID> Renderable::GetCameraRenderPasses(CameraID cam)
	{
		if (!cam->useSwapChain()) return cam->renderPassesUUID;

		std::vector<RenderPassInstanceID> rpiv = cam->renderPassesUUID;
		rpiv.push_back(renderer->swapChainPass);
		return rpiv;
	}

	void Renderable::CreateMaterialsInstances(CameraID cam)
	{
		for (auto& rp : GetCameraRenderPasses(cam))
		{
			CreateRenderPassMaterialsInstances(rp);
		}
	}

	void Renderable::CreateRenderPassMaterialsInstances(RenderPassInstanceID pass)
	{
		std::vector<PassMaterialOverride> pmo = passMaterialOverrides();

		if (materials.contains(pass))
		{
			return;
		}

		// if (!model3D.empty())
		// {
		// 	for (unsigned int i = 0; i < meshes.size(); i++)
		// 	{
		// 		std::vector<PassMaterialOverride> mpmo;
		// 		std::copy_if(pmo.begin(), pmo.end(), std::back_inserter(mpmo), [i](PassMaterialOverride& o) { return o.meshIndex == i; });
		// 		auto& mesh = meshes.at(i);
		// 		JUUID matUUID = model3D->materials.at(i)();
		// 		materials[pass].push_back(pass->GetRenderPassMaterialInstance(unit, matUUID, mesh, shadowed(), mpmo, uuid()));
		// 	}
		// }
		// else
		if (!meshMaterial().mesh.empty() && meshMaterial().mesh.contains("primitive") && !meshMaterial().mesh.at("primitive").empty() && !meshMaterial().materialUUID.empty())
		{
			std::vector<PassMaterialOverride> mpmo;
			std::copy_if(pmo.begin(), pmo.end(), std::back_inserter(mpmo), [](PassMaterialOverride& o) { return o.meshIndex == 0; });
			auto& mesh = meshes.at(0);
			JUUID matUUID = meshMaterial().materialUUID;
			if (!MaterialTemplateExist(matUUID))
			{
				matUUID = GetMaterialUUIDByName(fallbackMaterialName); //fallback
			}
			materials[pass].push_back(pass->GetRenderPassMaterialInstance(unit, matUUID, mesh, shadowed(), mpmo, uuid()));
		}
	}

	void Renderable::DestroyMaterialsInstances(CameraID cam)
	{
		for (auto& rp : GetCameraRenderPasses(cam))
		{
			DestroyRenderPassMaterialsInstances(rp);
		}
	}

	void Renderable::DestroyRenderPassMaterialsInstances(RenderPassInstanceID rp)
	{
		// if (!materials.contains(rp))
		// {
		// 	return;
		// }
		// for (auto& m : materials.at(rp))
		// {
		// 	DeleteMaterialInstance(m());
		// }
		// materials.erase(rp);
	}

	//Pipelines
	void Renderable::CreatePipelines(CameraID cam)
	{
		for (auto& rp : GetCameraRenderPasses(cam))
		{
			CreateRenderPassPipelines(rp);
		}
	}	

	void Renderable::CreateRenderPassPipelines(RenderPassInstanceID pass)
	{
		RenderableID r(uuid());
		
		GetSceneUnit(unit)->PushToPreRender([r,pass]{
			
			using namespace ShaderCompiler;

			auto& meshes = r->meshes;
			for(unsigned int i=0; i < meshes.size(); i++)
			{
				auto& material = r->materials[pass].at(i);
				
				//create the bindgroup pipeline layout
				std::vector<wgpu::BindGroupLayout> bindGroupLayouts;
				for(unsigned int group = 0; group < r->bindGroupsLayout[pass].at(i).size(); group++) {
					bindGroupLayouts.push_back(r->bindGroupsLayout[pass].at(i).at(group));
				}

				wgpu::PipelineLayoutDescriptor pipelineLayoutDesc{};
				pipelineLayoutDesc.bindGroupLayoutCount = bindGroupLayouts.size();
				pipelineLayoutDesc.bindGroupLayouts = bindGroupLayouts.data();

				wgpu::PipelineLayout pipelineLayout = renderer->GetDevice().CreatePipelineLayout(&pipelineLayoutDesc);

				//create the color target state
				std::vector<wgpu::ColorTargetState> colorTargetStates;
				auto formats = pass->renderPassTemplate->renderTargetFormats();
				std::transform(formats.begin(), formats.end(), std::back_inserter(colorTargetStates), [](auto f){ return wgpu::ColorTargetState({.format=f}); });
				
				//the vertex buffer layout
				wgpu::VertexBufferLayout vertexBufferLayout = GetVertexBufferLayout(meshes.at(i)->vertexClass);
				wgpu::VertexState vertexState {
					.module = material->vertexShaderInstanceID->shaderModule,
					.entryPoint = shaderEntryPoint.at(VERTEX_SHADER).c_str(),
					.bufferCount = 1,
					.buffers = &vertexBufferLayout
				};

				//the fragment state
				wgpu::FragmentState fragmentState {
      				.module = material->pixelShaderInstanceID->shaderModule,
					.entryPoint = shaderEntryPoint.at(PIXEL_SHADER).c_str(),
					.targetCount = colorTargetStates.size(),
					.targets = colorTargetStates.data()
				};
				
				//and the descriptor
				wgpu::RenderPipelineDescriptor descriptor{
					.layout = pipelineLayout,
					.vertex = vertexState,
					.fragment = &fragmentState,
				};

				wgpu::DepthStencilState depthStencilState;

				if(pass->renderPassTemplate->depthStencilFormat() != wgpu::TextureFormat::Undefined)
				{
					depthStencilState.format = pass->renderPassTemplate->depthStencilFormat(),
					depthStencilState.depthWriteEnabled = true,
					depthStencilState.depthCompare = wgpu::CompareFunction::Less,
					
					depthStencilState.stencilFront.compare = wgpu::CompareFunction::Always;
					depthStencilState.stencilBack.compare = wgpu::CompareFunction::Always;
					depthStencilState.stencilReadMask = 0xFFFFFFFF;
					depthStencilState.stencilWriteMask = 0xFFFFFFFF;

					descriptor.depthStencil = &depthStencilState;
				}

  				r->pipelines[pass].push_back(renderer->GetDevice().CreateRenderPipeline(&descriptor));
			}
		});
	}	

	void Renderable::DestroyPipelines(CameraID cam)
	{
		for (auto& rp : GetCameraRenderPasses(cam))
		{
			DestroyRenderPassPipelines(rp);
		}
	}	

	void Renderable::DestroyRenderPassPipelines(RenderPassInstanceID pass)
	{

	}

	//Uniform Buffers
	void Renderable::CreateUniformBuffersInstances(CameraID cam)
	{
		for (auto& rp : GetCameraRenderPasses(cam))
		{
			CreateRenderPassUniformBuffersInstances(rp);
		}
	}

	void Renderable::CreateRenderPassUniformBuffersInstances(RenderPassInstanceID pass)
	{
		uniformBuffers[pass]={};
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			auto& mi = materials[pass].at(i);
			auto& mesh = meshes.at(i);

			std::string matStr = MaterialUniformBufferName;

			UniformBufferID matBuffer = mi->CreateUniformBuffers(unit, matStr);
			uniformBuffers[pass].push_back(matBuffer);
		}
		RenderableID r(uuid());
		GetSceneUnit(unit)->PushToPreRender([r,pass]{
			for (unsigned int i = 0; i < r->meshes.size(); i++)
			{
				auto& mi = r->materials[pass].at(i);
				auto& ub = r->uniformBuffers[pass].at(i);
				ub->push(mi->materialAttributes,0);
			}
		});
	}

	void Renderable::DestroyUniformBuffersInstances(CameraID cam)
	{
		// for (unsigned int i = 0; i < JRenderer::numFrames; i++)
		// {
		// 	if (constantsBuffersLock[i]->load() == true)
		// 		constantsBuffersLock[i]->wait(true);
		// }

		// for (unsigned int i = 0; i < JRenderer::numFrames; i++)
		// {
		// 	constantsBuffersLock[i]->store(true);
		// }

		for (auto& rp : GetCameraRenderPasses(cam))
		{
			DestroyRenderPassUniformBuffersInstances(rp);
		}

		// for (unsigned int i = 0; i < JRenderer::numFrames; i++)
		// {
		// 	constantsBuffersLock[i]->store(false);
		// 	constantsBuffersLock[i]->notify_one();
		// }
	}

	void Renderable::DestroyRenderPassUniformBuffersInstances(RenderPassInstanceID pass)
	{
		if (!uniformBuffers.contains(pass)) return;

		// for (unsigned int i = 0; i < uniformBuffers.at(pass).size(); i++)
		// {
		// 	for (unsigned int c = 0; c < uniformBuffers.at(pass).at(i).size(); c++)
		// 	{
		// 		DestroyUniformBuffer(uniformBuffers.at(pass).at(i)[c]());
		// 	}
		// }
		uniformBuffers.erase(pass);
	}

	void Renderable::CreateBindGroups(CameraID cam)
	{
		for (auto& rp : GetCameraRenderPasses(cam))
		{
			if (bindGroups.contains(rp)) bindGroups.erase(rp);
			CreateRenderPassBindGroups(rp);
		}
	}

	void Renderable::CreateRenderPassBindGroups(RenderPassInstanceID rp)
    {
		using namespace Templates::Texture;

        RenderableID r(uuid());

        GetSceneUnit(unit)->PushToPreRender([r,rp]{

            std::unordered_map<std::string, std::function<UniformBufferID(RenderPassInstanceID rp, unsigned int meshId)>> getUniformBuffer = {
                { CameraUniformBufferName, [&](RenderPassInstanceID rp, unsigned int meshId) {
                    if(!rp->camera && rp->renderPassTemplate->type() == RenderPassType_SwapChainPass && rp->renderPassTemplate->renderCallbackOverride() != RenderPassRenderCallbackOverride_Resolve)
                        return CameraID(*(GetSwapChainCameras(r->unit).begin()))->cameraUb;
                    return rp->camera->cameraUb;
                } },
                { LightUniformBufferName, [&](RenderPassInstanceID rp, unsigned int meshId) { 
					if(!rp->camera && rp->renderPassTemplate->type() == RenderPassType_SwapChainPass && rp->renderPassTemplate->renderCallbackOverride() != RenderPassRenderCallbackOverride_Resolve)
                        return CameraID(*(GetSwapChainCameras(r->unit).begin()))->lightsUb;
                    return rp->camera->lightsUb;
				 } },
                { ShadowMapUniformBufferName, [&](RenderPassInstanceID rp, unsigned int meshId) { return UniformBufferID(""); } },
                { ModelUniformBufferName, [&](RenderPassInstanceID rp, unsigned int meshId){ return r->worldUb; } },
                { MaterialUniformBufferName, [&](RenderPassInstanceID rp, unsigned int meshId){ return r->uniformBuffers[rp][meshId]; } },
                { AnimationUniformBufferName, [&](RenderPassInstanceID rp, unsigned int meshId) { return UniformBufferID(""); } },
            };

			std::unordered_map<std::string, TextureShaderUsage> bindingToTextureShaderUsage = {
				//TextureShaderUsage_IBLIrradiance,
				//TextureShaderUsage_IBLPreFilteredEnvironment,
				//TextureShaderUsage_IBLBRDFLUT,
				{ baseTexture, TextureShaderUsage_Base },
				//TextureShaderUsage_NormalMap,
				//TextureShaderUsage_MetallicRoughness,
				//TextureShaderUsage_ShadowMaps,
				//TextureShaderUsage_MinTexture,
				//TextureShaderUsage_MaxTexture,
				//TextureShaderUsage_DepthTexture,
				//TextureShaderUsage_AverageLuminance,
			};

            // Aseguramos que los contenedores principales tengan espacio para todos los meshes
            r->shaderBindings[rp].resize(r->meshes.size());
            r->bindGroupsLayout[rp].resize(r->meshes.size());
            r->bindGroups[rp].resize(r->meshes.size());

            for(unsigned int mesh = 0U; mesh < r->meshes.size(); mesh++)
            {
                auto& mi = r->materials[rp].at(mesh);
                r->shaderBindings[rp][mesh] = mi->GetShaderBindings();
                auto& bindings = r->shaderBindings[rp][mesh];

                // Preparamos los vectores para este mesh exacto
                auto& meshLayouts = r->bindGroupsLayout[rp][mesh];
                meshLayouts.clear();
                
                auto& meshBindGroups = r->bindGroups[rp][mesh];
                meshBindGroups.resize(bindings.size()); // Un vector de frames por cada grupo

                for(unsigned int group = 0; group < bindings.size(); group++)
                {
                    auto& sorted = bindings.at(group);  
                    std::vector<wgpu::BindGroupLayoutEntry> layoutEntries;
                    
                    for(auto b : sorted)
                    {
                        auto [g_idx, binding, buffer_size, multi_frame, storage_buffer, name, is_texture, is_sampler] = b;
                        
						wgpu::BindGroupLayoutEntry entry = {};
						entry.binding = binding;

						if (is_texture)
						{
							// Las texturas suelen consumirse en el fragment shader (o vertex si usas vertex texture fetch)
							entry.visibility = wgpu::ShaderStage::Fragment;
							entry.texture.sampleType = wgpu::TextureSampleType::Float;
							entry.texture.viewDimension = wgpu::TextureViewDimension::e2D;
							entry.texture.multisampled = false;
						} 
						else if (is_sampler)
						{
							// Los samplers también viven típicamente en el fragment shader
							entry.visibility = wgpu::ShaderStage::Fragment;
							entry.sampler.type = wgpu::SamplerBindingType::Filtering;
						} 
						else
						{
							// Uniform Buffers tradicionales (Vertex + Fragment)
							entry.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
							entry.buffer.type = storage_buffer ? wgpu::BufferBindingType::ReadOnlyStorage : wgpu::BufferBindingType::Uniform;
							entry.buffer.minBindingSize = buffer_size;
							entry.buffer.hasDynamicOffset = false;
						}

						layoutEntries.push_back(entry);
                    }

                    wgpu::BindGroupLayoutDescriptor layoutDesc = {};
                    layoutDesc.entryCount = layoutEntries.size();
                    layoutDesc.entries = layoutEntries.data();

                    std::string layoutLabel = "layout:" + std::to_string(group);
                    layoutDesc.label = layoutLabel.c_str();

                    wgpu::BindGroupLayout layout = renderer->GetDevice().CreateBindGroupLayout(&layoutDesc);
                    meshLayouts.push_back(layout); // Guardado ordenado en el índice del grupo

                    // Creamos los BindGroups para cada frame de ESTE grupo específico
                    auto& frameBindGroups = meshBindGroups[group];
                    frameBindGroups.clear();

                    for(unsigned int frame = 0; frame < JRenderer::numFrames; frame++)
                    {
                        std::vector<wgpu::BindGroupEntry> bgEntries;
                        for(auto b : sorted)
                        {
                            wgpu::BindGroupEntry entry;
                            auto [g_idx, binding, buffer_size, multi_frame, storage_buffer, name, is_texture, is_sampler] = b;
							
							entry.binding = binding;
							if(is_texture)
							{
								TextureInstanceID texInst = mi->textures.at(bindingToTextureShaderUsage.at(name));
								entry.textureView = texInst->textureView;
							}
							else if(is_sampler)
							{
								entry.sampler = mi->samplers.at(binding);
							}
							else
                            {
								entry.buffer = getUniformBuffer.at(name)(rp, mesh)->uniformBuffer;
								entry.offset = multi_frame ? (frame * ((buffer_size + 255) & ~255)) : 0U; 
								entry.size = buffer_size;
							}

                            bgEntries.push_back(entry);
                        }

                        wgpu::BindGroupDescriptor bgDesc = {};
                        bgDesc.entryCount = bgEntries.size();
                        bgDesc.entries = bgEntries.data();
                        bgDesc.layout = layout; // <-- ¡Vinculado estrictamente a su layout correcto!

                        std::string bgLabel = "group:" + std::to_string(group) + "-" + std::to_string(frame);
                        bgDesc.label = bgLabel.c_str();

                        frameBindGroups.push_back(renderer->GetDevice().CreateBindGroup(&bgDesc));
                    }
                }
            }
        });
    }

	void Renderable::DestroyBindGroups(CameraID cam)
	{
		/*
		if (cam.empty()) return;
		for (auto& rp : GetCameraRenderPasses(cam))
		{
			DestroyRenderPassRootSignatures(rp);
		}
		*/
	}

	void Renderable::DestroyRenderPassBindGroups(RenderPassInstanceID rp)
	{
		/*
		if (rootSignatures.contains(rp))
			rootSignatures.erase(rp);
		*/
	}

	void Renderable::CreateRenderMethods(CameraID cam)
	{
		RenderableID r(uuid());
		
		for (auto& rp : GetCameraRenderPasses(cam))
		{
			GetSceneUnit(unit)->PushToPreRender([rp,cam,r] {
				std::tuple<CameraID, RenderPassInstanceID> key = std::make_tuple(cam, rp);
				std::vector<std::vector<BindGroupSetter>> setters = r->CreateRenderPassRenderMethods(cam, rp);
				r->bindGroupRenders[key] = setters;
			});
		}
/*
#if defined(_EDITOR)
		//this is a complete hack, but what the hell
		using namespace Editor;
		if (levelCameraUUID.contains(unit) && cam == levelCameraUUID.at(unit))
		{
			CameraID edCam = editorCameraUUID.at(unit);
			for (auto& rp : GetCameraRenderPasses(cam))
			{
				std::tuple<CameraID, RenderPassInstanceID> key = std::make_tuple(edCam, rp);
				std::vector<std::vector<DescriptorTableSetter>> setters = CreateEditorCameraRenderPassRenderMethods(edCam, cam, rp);
				descriptorsRenders[key] = setters;
			}
		}
#endif
*/
	}

	std::vector<std::vector<BindGroupSetter>> Renderable::CreateRenderPassRenderMethods(CameraID cam, RenderPassInstanceID rp)
	{
		RenderableID r(uuid());
		std::vector<std::vector<BindGroupSetter>> setters; //[mesh][group]

		for (unsigned int mesh = 0; mesh < meshes.size(); mesh++)
		{
			setters.push_back({});
			auto& groupSetter = setters.back();
			auto& groupBindings = shaderBindings.at(rp).at(mesh);
			for(unsigned int group = 0U; group < groupBindings.size(); group++)
			{
				auto setter = [rp, mesh, group, r](wgpu::RenderPassEncoder& encoder, unsigned int backBufferIndex)
				{
					// Seleccionas el BindGroup pre-creado para este frame específico
					auto& frameBindGroups = r->bindGroups[rp].at(mesh).at(group); // asumiendo que guardaste por frame
					auto& g = frameBindGroups.at(backBufferIndex % frameBindGroups.size());

					// Llamada limpia de 2 parámetros: Cero punteros, cero arreglos, cero errores de Emscripten
					encoder.SetBindGroup(group, g);
				};

				groupSetter.push_back(setter);
			}
		}

		return setters;
	}
/*
#if defined(_EDITOR)
	std::vector<std::vector<DescriptorTableSetter>> Renderable::CreateEditorCameraRenderPassRenderMethods(CameraID edCam, CameraID cam, RenderPassInstanceID rp)
	{
		using namespace Animation;

		Camera* pcamera = (!cam.empty()) ? (*cam).get() : nullptr;
		Camera* pedCamera = (!edCam.empty()) ? (*edCam).get() : nullptr;
		auto& meshesMaterials = materials.at(rp);

		std::vector<std::vector<DescriptorTableSetter>> meshSetters;

		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			auto& material = meshesMaterials.at(i);
			MaterialInstance* material_ins = (*material).get();
			ShaderInstance* vertexShader_ins = (*material_ins->vertexShaderInstanceID).get();
			ShaderInstance* pixelShader_ins = (*material_ins->pixelShaderInstanceID).get();

			std::vector<DeviceUtils::ConstantsBufferID>& cbuffers = constantsBuffers.at(rp).at(i);
			std::vector<DeviceUtils::ConstantsBuffer*> pcbuffers;
			std::transform(cbuffers.begin(), cbuffers.end(), std::back_inserter(pcbuffers), [](auto& c)
				{
					return (*c).get();
				}
			);
			DeviceUtils::ConstantsBuffer* pcam_cbuffer = (pedCamera) ? (*pedCamera->cameraCb).get() : nullptr;
			DeviceUtils::ConstantsBuffer* pcam_light_cbuffer = (pcamera && pcamera->lightsCB) ? (*pcamera->lightsCB).get() : nullptr;
			DeviceUtils::ConstantsBuffer* pcam_shadowmap_cbuffer = (pcamera && pcamera->shadowMapsCB) ? (*pcamera->shadowMapsCB).get() : nullptr;
			DeviceUtils::ConstantsBuffer* animated_cbuffer = (!animable.empty()) ? (*GetAnimatedConstantsBuffer(SUuuid())).get() : nullptr;

			std::vector<std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE>> gpu_handles_cbuffers;
			std::transform(pcbuffers.begin(), pcbuffers.end(), std::back_inserter(gpu_handles_cbuffers), [](auto& c)
				{
					return c->gpu_xhandle;
				}
			);
			std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handles_camera;
			if (pcam_cbuffer)
			{
				gpu_handles_camera = pcam_cbuffer->gpu_xhandle;
			}
			std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handles_lights;
			if (pcam_light_cbuffer)
			{
				gpu_handles_lights = pcam_light_cbuffer->gpu_xhandle;
			}
			std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handles_shadowMaps;
			if (pcam_shadowmap_cbuffer)
			{
				gpu_handles_shadowMaps = pcam_shadowmap_cbuffer->gpu_xhandle;
			}
			std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handles_skinning;
			if (animated_cbuffer)
			{
				gpu_handles_skinning = animated_cbuffer->gpu_xhandle;
			}
			std::map<unsigned int, ::CD3DX12_GPU_DESCRIPTOR_HANDLE> mat_uav = material_ins->uav;

			CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_iblIrradiance = (pcamera && pcamera->iblTextures.contains(TextureShaderUsage_IBLIrradiance)) ? GetTextureInstance(pcamera->iblTextures.at(TextureShaderUsage_IBLIrradiance))->gpuHandle : CD3DX12_GPU_DESCRIPTOR_HANDLE();

			CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_iblPrefiteredEnv = (pcamera && pcamera->iblTextures.contains(TextureShaderUsage_IBLPreFilteredEnvironment)) ? GetTextureInstance(pcamera->iblTextures.at(TextureShaderUsage_IBLPreFilteredEnvironment))->gpuHandle : CD3DX12_GPU_DESCRIPTOR_HANDLE();

			CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_iblBRDFLUT = (pcamera && pcamera->iblTextures.contains(TextureShaderUsage_IBLBRDFLUT)) ? GetTextureInstance(pcamera->iblTextures.at(TextureShaderUsage_IBLBRDFLUT))->gpuHandle : CD3DX12_GPU_DESCRIPTOR_HANDLE();

			std::vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handle_srv;
			for (auto& [textureType, texParam] : pixelShader_ins->srvTexParameters)
			{
				if (texParam.numSRV == 0xFFFFFFFF || iblUsageTexture.contains(textureType)) continue;
				auto& texInstance = GetTextureInstance(material_ins->textures.at(textureType));
				gpu_handle_srv.push_back(texInstance->gpuHandle);
			}

			CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_srv_shadowMap = (pcamera && pcamera->SceneHasShadowMaps()) ? GetShadowMapGpuDescriptorHandleStart(unit) : CD3DX12_GPU_DESCRIPTOR_HANDLE();

			std::vector<DescriptorTableSetter> descriptor_table_setters;

			//f2f
			SetConstantsBuffersDescriptorTables(descriptor_table_setters, gpu_handles_cbuffers);
			SetCameraConstantsBufferDescriptorTable(descriptor_table_setters, gpu_handles_camera, vertexShader_ins, pixelShader_ins);
			SetLightsConstantsBufferDescriptorTable(descriptor_table_setters, gpu_handles_lights, vertexShader_ins, pixelShader_ins);
			SetShadowMapsConstantsBufferDescriptorTable(descriptor_table_setters, pcamera, gpu_handles_shadowMaps, vertexShader_ins, pixelShader_ins);
			SetSkinningConstantsBufferDescriptorTable(descriptor_table_setters, gpu_handles_skinning, vertexShader_ins, pixelShader_ins);
			//not f2f
			SetUAVRootDescriptorTable(descriptor_table_setters, mat_uav, pixelShader_ins);
			SetIBLRootDescriptorTable(descriptor_table_setters, vertexShader_ins, pixelShader_ins, gpu_handle_iblIrradiance, gpu_handle_iblPrefiteredEnv, gpu_handle_iblBRDFLUT);
			SetSRVRootDescriptorTable(descriptor_table_setters, gpu_handle_srv);
			SetShadowMapsSRVDescriptorTable(descriptor_table_setters, pcamera, vertexShader_ins, pixelShader_ins, gpu_handle_srv_shadowMap);

			meshSetters.push_back(descriptor_table_setters);
		}
		return meshSetters;
	}
#endif
*/
/*
	void Renderable::SetConstantsBuffersDescriptorTables(std::vector<DescriptorTableSetter>& setters, std::vector<std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE>> gpu_handles_cbuffers)
	{
		setters.push_back([gpu_handles_cbuffers](CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)
			{
				for (auto gpu_xhandle : gpu_handles_cbuffers) {
					commandList->SetGraphicsRootDescriptorTable(slot, gpu_xhandle[backBufferIndex]);
					slot++;
				}
			});
	};
*/
/*
	void Renderable::SetCameraConstantsBufferDescriptorTable(std::vector<DescriptorTableSetter>& setters, std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handles_camera, ShaderInstance* vertexShader_ins, ShaderInstance* pixelShader_ins)
	{
		if (!gpu_handles_camera.empty() && (vertexShader_ins->CBV.camera != -1 || pixelShader_ins->CBV.camera != -1)) {
			setters.push_back([gpu_handles_camera](
				CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)
				{
					commandList->SetGraphicsRootDescriptorTable(slot++, gpu_handles_camera[backBufferIndex]);
				});
		}
	};
*/
/*
	void Renderable::SetLightsConstantsBufferDescriptorTable(std::vector<DescriptorTableSetter>& setters, std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handles_lights, ShaderInstance* vertexShader_ins, ShaderInstance* pixelShader_ins)
	{
		if (vertexShader_ins->CBV.light != -1 || pixelShader_ins->CBV.light != -1) {
			setters.push_back([gpu_handles_lights](
				CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)
				{
					commandList->SetGraphicsRootDescriptorTable(slot++, gpu_handles_lights[backBufferIndex]);
				});
		}
	};
*/
/*
	void Renderable::SetShadowMapsConstantsBufferDescriptorTable(std::vector<DescriptorTableSetter>& setters, Camera* camera, std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handles_shadowMaps, ShaderInstance* vertexShader_ins, ShaderInstance* pixelShader_ins)
	{
		if (vertexShader_ins->CBV.lightsShadowMap != -1 || pixelShader_ins->CBV.lightsShadowMap != -1) {
			setters.push_back([camera, gpu_handles_shadowMaps](
				CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)
				{
					if (camera->SceneHasShadowMaps())
						commandList->SetGraphicsRootDescriptorTable(slot, gpu_handles_shadowMaps[backBufferIndex]);
					slot++;
				});
		}
	};
*/
/*
	void Renderable::SetSkinningConstantsBufferDescriptorTable(std::vector<DescriptorTableSetter>& setters, std::vector<::CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handles_skinning, ShaderInstance* vertexShader_ins, ShaderInstance* pixelShader_ins)
	{
		if (vertexShader_ins->CBV.animation != -1 || pixelShader_ins->CBV.animation != -1) {
			setters.push_back([gpu_handles_skinning](CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)
				{
					if (!gpu_handles_skinning.empty())
						commandList->SetGraphicsRootDescriptorTable(slot, gpu_handles_skinning[backBufferIndex]);
					slot++;
				});
		}
	};
*/
/*
	void Renderable::SetUAVRootDescriptorTable(std::vector<DescriptorTableSetter>& setters, std::map<unsigned int, ::CD3DX12_GPU_DESCRIPTOR_HANDLE> uav, ShaderInstance* pixelShader_ins)
	{
		setters.push_back([uav, pixelShader_ins](CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)
			{

				for (auto& [name, uavParam] : pixelShader_ins->uavParameters)
				{
					if (uav.contains(uavParam.registerId))
					{
						commandList->SetGraphicsRootDescriptorTable(slot, uav.at(uavParam.registerId));
						slot++;
					}
				}
			});
	};
*/
/*
	void Renderable::SetIBLRootDescriptorTable(std::vector<DescriptorTableSetter>& setters, ShaderInstance* vertexShader_ins, ShaderInstance* pixelShader_ins, CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_iblIrradiance, CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_iblPrefiteredEnv, CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_iblBRDFLUT)
	{
		if ((
			vertexShader_ins->SRV.iblIrradiance != -1 &&
			vertexShader_ins->SRV.iblPrefiteredEnv != -1 &&
			vertexShader_ins->SRV.iblBRDFLUT != -1
			) ||
			(
				pixelShader_ins->SRV.iblIrradiance != -1 &&
				pixelShader_ins->SRV.iblPrefiteredEnv != -1 &&
				pixelShader_ins->SRV.iblBRDFLUT != -1)
			)
		{
			setters.push_back([gpu_handle_iblIrradiance, gpu_handle_iblPrefiteredEnv, gpu_handle_iblBRDFLUT](CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)
				{
					commandList->SetGraphicsRootDescriptorTable(slot++, gpu_handle_iblIrradiance);
					commandList->SetGraphicsRootDescriptorTable(slot++, gpu_handle_iblPrefiteredEnv);
					commandList->SetGraphicsRootDescriptorTable(slot++, gpu_handle_iblBRDFLUT);
				});

		}
	};
*/
/*
	void Renderable::SetSRVRootDescriptorTable(std::vector<DescriptorTableSetter>& setters, std::vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> gpu_handles_srv)
	{
		setters.push_back([gpu_handles_srv](CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)
			{
				for (CD3DX12_GPU_DESCRIPTOR_HANDLE handle : gpu_handles_srv)
				{
					commandList->SetGraphicsRootDescriptorTable(slot++, handle);
				}
			}
		);
	};
*/
/*
	void Renderable::SetShadowMapsSRVDescriptorTable(std::vector<DescriptorTableSetter>& setters, Camera* camera, ShaderInstance* vertexShader_ins, ShaderInstance* pixelShader_ins, CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle_srv_shadowMap)
	{
		if (vertexShader_ins->SRV.lightsShadowMap != -1 || pixelShader_ins->SRV.lightsShadowMap != -1) {
			setters.push_back([camera, gpu_handle_srv_shadowMap](
				CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)
				{
					if (camera->SceneHasShadowMaps())
						return commandList->SetGraphicsRootDescriptorTable(slot, gpu_handle_srv_shadowMap);
					slot++;
				}
			);
		}
	};
*/

	void Renderable::CreateBoundingBox()
	{
		using namespace Templates;

		bool extend = false;
		for (auto& mesh : meshes)
		{
			mesh->ExtendBoundingBox(boundingBox, extend);
			extend = true;
		}
	}

	bool Renderable::HasBoundingBoxComputed()
	{
		//return (!animable.empty()) ? boundingBoxCompute->hasSolution : true;
		return true;
	}

	BoundingBox Renderable::GetBoundingBox()
	{
		// if (!animable)
		// {
		// 	BoundingBox& bb = boundingBox;
		// 	BoundingBox bbw;
		// 	bb.Transform(bbw, world());
		// 	return bbw;
		// }
		// else
		// {
			BoundingBox bb(AABBCenter(), AABBExtent());
			BoundingBox bbw;
			bb.Transform(bbw, world());
			return bbw;
		// }
		
		// BoundingBox& bb = animable.empty() ? boundingBox : boundingBoxCompute->boundingBox;
		// BoundingBox bbw;
		// bb.Transform(bbw, world());
		// return bbw;
		
	}

	/*
	void Renderable::WriteMaterialVariablesToConstantsBufferSpace(MaterialInstanceID material, ConstantsBufferID cbvData, unsigned int cbvFrameIndex)
	{
		for (auto& [varName, varMapping] : material->variablesMapping)
		{
			UINT8* source = material->variablesBuffer[varMapping.mapping.bufferIndex].data() + varMapping.mapping.offset;
			UINT8* destination = cbvData->mappedConstantBuffer + (cbvFrameIndex * cbvData->alignedConstantBufferSize) + varMapping.mapping.offset;
			memcpy(destination, source, varMapping.mapping.size);
		}
	}
*/
/*
	void Renderable::WriteConstantsBuffer(std::string constantName, void* data, unsigned int backbufferIndex, unsigned int slot, size_t offset)
	{
		if (constantsBuffersLock[backbufferIndex]->load() == true)
			return;
		if (!constantsWriter.contains(constantName))
			return;
		auto& writers = constantsWriter.at(constantName);
		for (auto& writer : writers)
		{
			writer(data, backbufferIndex, offset, slot);
		}
	}
*/
/*
	void Renderable::WriteAnimationConstantsBuffer(unsigned int frame)
	{
		if (animable.empty()) return;

		using namespace Animation;
		WriteBoneTransformationsToConstantsBuffer(SUuuid(), bonesTransformation, frame);
	}
*/

	void Renderable::WriteUniformBuffer(unsigned int frame)
	{
		XMMATRIX w = world();
		worldUb->push(w,frame);
		//WriteConstantsBuffer("world", &w, frame);
	}
	
	//Animation
	/*
	void Renderable::CreateAnimationSequences()
	{
		std::unique_ptr<Model3DJson>& mdl = GetModel3DTemplate(model());
		animationsSequences = mdl->animationSequences();
		for (auto& [name, time] : animable->animations->animationsLength)
		{
			int totalFrames = static_cast<int>(time * 60);
			totalFrames /= 1000;
			SequenceChannel seqChannel;
			seqChannel.name = "animation";
			ChannelElement channelElement;
			channelElement.type = SCET_Animation;
			SequenceChannelElementAnimation& animation = channelElement.animation;
			animation.animation = name;
			animation.startTime = 0.0f;
			animation.endTime = time;
			animation.frameStart = 0;
			animation.frameEnd = totalFrames;
			seqChannel.elements.push_back(channelElement);
			Sequence sequence;
			sequence.framesPerSecond = 60;
			sequence.totalFrames = totalFrames;
			sequence.sequenceChannels.push_back(seqChannel);
			animationsSequences.sequences.insert_or_assign(name, sequence);
		}
	}
*/
/*
	void Renderable::RebuildAnimationSequences()
	{
		CreateAnimationSequences();
	}
*/
/*
	void Renderable::SetCurrentAnimation(SequencePlayer* seqPlayer)
	{
		if (seqPlayer == nullptr)
		{
			if (animationPlayers.contains(MAKESUUUID(unit, uuid())))
			{
				animationPlayers.erase(MAKESUUUID(unit, uuid()));
			}
		}
		else
		{
			animationPlayers.insert_or_assign(MAKESUUUID(unit, uuid()), seqPlayer);
		}
	}
*/
/*
	void Renderable::SetCurrentAnimation(std::string anim, float startTime, float timeFactor, bool play, bool loop)
	{
		if (!animationPlayers.contains(MAKESUUUID(unit, uuid())) || animationPlayers.at(MAKESUUUID(unit, uuid())) != &sequencePlayer)
			SetCurrentAnimation(&sequencePlayer);

		animationSequence(anim);
		sequencePlayer.DestroySequenceTriggers();
		sequencePlayer.sequence = animationsSequences.sequences.at(anim);
		sequencePlayer.loop = loop;
		sequencePlayer.newSequence = true;
		sequencePlayer.ResetFrames();
		sequencePlayer.ApplyFrameValues();
		sequencePlayer.CreateSequenceTriggers();
		StepAnimation(0.0f);
		animationTimeFactor(timeFactor);
		forceAnimation = true;
	}
*/
/*
	void Renderable::CreateAnimationThread()
	{
		animationThreadAlive = std::make_unique<std::atomic_bool>(true);

		// Guardamos la hebra SIN hacer detach()
		animationThread = std::thread([](Renderable* r)
			{
				auto& scene = GetSceneUnit(r->unit);
				std::string animationThreadEvent = "CreateAnimationThread:" + r->name() + ":" + r->uuid();

				while (r->animationThreadAlive->load(std::memory_order_relaxed))
				{
					using namespace Animation;

					// Espera activa/suspendida MIENTRAS sea false (esperando trabajo)
					while (!r->animationStepLock->load(std::memory_order_acquire) &&
						r->animationThreadAlive->load(std::memory_order_relaxed))
					{
						r->animationStepLock->wait(false, std::memory_order_relaxed);
					}

					if (!r->animationThreadAlive->load(std::memory_order_relaxed))
						break;

					auto& animations = r->animable->animations;
#if defined(_DEVELOPMENT)
					PIXScopedEvent(0, nostd::StringToWString(animationThreadEvent).c_str());
#endif
					TraverseMultiplycationQueue(r->animationTime(), r->animation(), animations, r->bonesTransformation, r->sequenceBoneTransformations, r->globalNodeTransforms);

					// Tarea completada: volvemos a false y notificamos a los consumidores
					r->animationStepLock->store(false, std::memory_order_release);
					r->animationStepLock->notify_all(); // notify_all en caso de que SequencePlayer y el hilo principal esperen
				}
			}, this);
	}
*/
/*
	void Renderable::StepAnimation(double elapsedSeconds)
	{
		if (animable.empty()) return;

		if (lastAnimationTime == animationTime() && !forceAnimation)
			return;

		forceAnimation = false;
		lastAnimationTime = animationTime();

		// Esperar a que el hilo de animación termine cualquier frame previo (debe ser false)
		while (animationStepLock->load(std::memory_order_acquire))
		{
			animationStepLock->wait(true, std::memory_order_relaxed);
		}

		// Activar la señal de procesamiento (true)
		animationStepLock->store(true, std::memory_order_release);
		animationStepLock->notify_one();
	}
*/
/*
	std::tuple<XMMATRIX, XMFLOAT3, XMFLOAT3, XMVECTOR, XMFLOAT3> Renderable::GetBoneTransformation(std::string bone)
	{
		if (bone.empty())
		{
			return std::make_tuple<XMMATRIX, XMFLOAT3, XMFLOAT3, XMVECTOR, XMFLOAT3>(
				XMMatrixIdentity(),
				{ 0.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 0.0f },
				XMQuaternionIdentity(),
				{ 1.0f, 1.0f, 1.0f }
			);
		}
		auto& nodesTransforms = globalNodeTransforms;
		return Animation::GetBoneTransformation(world(), nodesTransforms, bone);
	}
	*/
/*
	std::vector<std::string> Renderable::GetBones()
	{
		std::vector<std::string> bones = nostd::GetKeysFromMap(animable->animations->bonesParents);
		std::sort(bones.begin(), bones.end(), [](std::string a, std::string b) { return a < b; });
		return bones;
	}
	*/
	/*
	void Renderable::Destroy()
	{
		using namespace ComputeShader;

		if (animationThreadAlive)
		{
			// 1. Notificar apagado
			animationThreadAlive->store(false, std::memory_order_release);

			// 2. Despertar al hilo si estaba dormido
			animationStepLock->store(true, std::memory_order_release);
			animationStepLock->notify_all();

			// 3. Esperar la terminación de forma segura
			if (animationThread.joinable())
			{
				animationThread.join();
			}
		}

		for (auto& [rp, vec0] : materials)
		{
			for (auto& mat : vec0)
			{
				DeleteMaterialInstance(mat());
			}
		}
		materials.clear();

		for (auto& [rp, vec0] : constantsBuffers)
		{
			for (auto& vec1 : vec0)
			{
				for (auto& cbuffer : vec1)
				{
					DestroyConstantsBuffer(cbuffer());
				}
			}
		}
		constantsBuffers.clear();

		auto destroyMeshInstance = [](auto& vec) { for (auto& mesh : vec) { DestroyMeshInstance(mesh()); } };

		if (model3D.empty())
		{
			destroyMeshInstance(meshes);
		}
		meshes.clear();

		if (!model3D.empty())
		{
#if defined(_EDITOR)
			Model3DJsonID model3dJson = model();
			model3dJson->RemoveUpdateListener(Model3DJson::Update_animationSequences, SUuuid());
#endif

			DeleteModel3DInstance(model3D());
		}

		auto list = GetPhysicsObjectsBySceneObjectUUID(SUuuid());
		for (PhysicObjectID phO : list)
		{
			phO->markedForDelete = true;
		}

#include <Attributes/JDestroy.h>
#include "RenderableAtt.h"
#include <JEnd.h>

		SceneObject::Destroy();
	}
	*/

	//RENDER
	void Renderable::Render(SceneUnitId unit, wgpu::RenderPassEncoder& pass, RenderPassInstanceID renderPass, CameraID camera)
	{
		// using namespace Animation;
		using namespace Scene;

		if (/*!RenderReady() || markedForDelete ||*/ !visible() || !materials.contains(renderPass) || !pipelines.contains(renderPass) /* || renderException*/) return;

		std::tuple<CameraID, RenderPassInstanceID> key = std::make_tuple(camera, renderPass);

		if (!bindGroupRenders.contains(key))
			return;

		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			auto& bindGroupSetters = bindGroupRenders.at(key).at(i);
			if (skipMeshes_contains(i)) continue;

			auto& mesh = meshes.at(i);
			pass.SetPipeline(pipelines.at(renderPass).at(i));
			pass.SetVertexBuffer(0, mesh->vertexBufferId->vertexBuffer);
			pass.SetIndexBuffer(mesh->indexBufferId->indexBuffer, IndexFormat::Uint32);

			for(unsigned int j = 0; j < bindGroupSetters.size(); j++)
			{
				bindGroupSetters.at(j)(pass, renderer->GetBackBufferIndex());
			}
			pass.DrawIndexed(mesh->indexBufferId->indexCount);
		}
	}

	/*
	bool Renderable::RenderReady()
	{
		return renderReady;
	}
	*/

	/*
	void Renderable::RenderReady(bool value)
	{
		renderReady = value;
	}
	*/

	/*
	std::vector<ScriptBinding> Renderable::GetScriptBindings()
	{
		std::vector<ScriptBinding> bindings;
		for (auto& [name, _] : at("controllers").items())
		{
			bindings.push_back(ScriptBinding(uuid(), name, name));
		}
		return bindings;
	}
	*/

	/*
#if defined(_EDITOR)
	std::map<std::string, ScriptBinding> Renderable::GetScriptBindingOptions()
	{
		std::map<std::string, ScriptBinding> options = SceneObject::GetScriptBindingOptions();

		for (auto& [key, _] : at("controllers").items())
		{
			std::string name = std::string(at("name")) + "/" + std::string(key);
			options.insert_or_assign(name, ScriptBinding(at("uuid"), key));
		}

		for (unsigned int i = 0; i < at("physicObject").size(); i++)
		{
			std::string name = std::string(at("name")) + "/physicObject/" + std::to_string(i);
			options.insert_or_assign(name, ScriptBinding(at("uuid"), i));
		}

		return options;
	}
#endif
	*/

	/*
	void RenderablesStep(SceneUnitId id, float dt)
	{
#if defined(_DEVELOPMENT)
		std::string event = std::string(__FUNCTION__) + ":" + std::to_string(id);
		PIXScopedEvent(0, nostd::StringToWString(event).c_str());
#endif
		auto& Renderables = GetRenderables(id);
		std::set<RenderableID> r;
		std::transform(Renderables.begin(), Renderables.end(), std::inserter(r, r.begin()), [&](auto o) { return MAKESUUUID(id, o); });
		std::erase_if(r, [](RenderableID r) { return !SceneObjectExists(r()) || !r->RenderReady(); });

		auto checkCamera = [](RenderableID r)
			{
				if (!SceneObjectExists(r())) return;
				if (!r->dirty(Renderable::Update_cameras)) return;
				r->clean(Renderable::Update_cameras);

				auto& prev = r->UpdatePrevValues.at("cameras");
				std::vector<std::string> prevCams;
				for (auto& it : prev.items())
				{
					if (it.value() != "")
					{
						prevCams.push_back(it.value());
					}
				}
				auto& curr = r->at("cameras");
				std::vector<std::string> currCams;
				for (auto& it : curr.items())
				{
					if (it.value() != "")
					{
						currCams.push_back(it.value());
					}
				}
				if (prevCams != currCams)
				{
					LoadingProcessor loading = CreateLoadingProcessor();
					r->UnbindCameras();
					r->BindShadowMapCameras();
					r->BindCameras();
				}
			};

		std::for_each(r.begin(), r.end(), checkCamera);

		std::set<RenderableID> cleanRot;
		std::for_each(r.begin(), r.end(), [&](auto& o)
			{
				if (o->dirty(Renderable::Update_rotation))
				{
					o->updateRotationQ();
					cleanRot.insert(o);
				}
			}
		);

		std::set<RenderableID> rGPose;
		std::copy_if(r.begin(), r.end(), std::inserter(rGPose, rGPose.begin()), [](auto& o)
			{
				bool updated = o->dirty(Renderable::Update_position) || o->dirty(Renderable::Update_rotation);
				return updated && !o->at("physicObject").empty();
			}
		);

		for (auto& rp : rGPose)
		{
			rp->clean(Renderable::Update_position);
			rp->clean(Renderable::Update_rotation);
			for (PhysicObjectID phO : GetPhysicsObjectsBySceneObjectUUID(rp->SUuuid()))
			{
				phO->UpdateGlobalPoseFromRenderable();
			}
		}

		std::set<RenderableID> rGeom;
		std::copy_if(r.begin(), r.end(), std::inserter(rGeom, rGeom.begin()), [](auto o)
			{
				if (!SceneObjectExists(o()))
					return false;
				bool updated = o->dirty(Renderable::Update_scale);
				o->clean(Renderable::Update_scale);
				return updated && !o->at("physicObject").empty();
			}
		);

		for (auto& rp : rGeom)
		{
			for (PhysicObjectID phO : GetPhysicsObjectsBySceneObjectUUID(rp->SUuuid()))
			{
			}
		}

		std::set<RenderableID> rMeshMaterial;
		std::copy_if(r.begin(), r.end(), std::inserter(rMeshMaterial, rMeshMaterial.begin()), [](auto& o)
			{
				return o->dirty(Renderable::Update_meshMaterial);
			}
		);

		if (rMeshMaterial.size() > 0)
		{
			for (auto o : rMeshMaterial)
			{
				o->clean(Renderable::Update_meshMaterial);
			}
		}

		std::set<RenderableID> rDepthStencil;
		std::copy_if(r.begin(), r.end(), std::inserter(rDepthStencil, rDepthStencil.begin()), [](auto& o)
			{
				return o->dirty(Renderable::Update_depthStencil);
			}
		);

		if (rDepthStencil.size() > 0)
		{
			for (auto o : rDepthStencil)
			{
				o->clean(Renderable::Update_depthStencil);
			}
		}

		for (auto o : cleanRot)
		{
			o->clean(Renderable::Update_rotation);
		}

		std::set<RenderableID> todelete;
		std::copy_if(r.begin(), r.end(), std::inserter(todelete, todelete.begin()), [](auto r)
			{
				return r->markedForDelete;
			}
		);

		for (auto renderable : todelete)
		{
			if (renderable->deleteFrames > 0U)
			{
				renderable->deleteFrames--;
				continue;
			}

			auto list = GetPhysicsObjectsBySceneObjectUUID(renderable->SUuuid());
			for (PhysicObjectID phO : list)
			{
#if defined(_EDITOR)
				phO->DestroyPhysicsAvatar();
#endif
				phO->markedForDelete = true;
				renderable->clear();
			}
			EraseRenderableFromRenderables(renderable->unit, renderable.uuid());
			EraseRenderableFromAnimables(renderable->unit, renderable.uuid());
			EraseRenderableFromShadowCasts(renderable->unit, renderable.uuid());
			DeleteRenderableSceneObject(renderable);
		}

		for (auto& [renderable, player] : animationPlayers)
		{
			if (renderable->markedForDelete || !renderable->RenderReady())
				continue;

#if defined(_EDITOR)
			if (Editor::IsPlaying(renderable->unit) && !Editor::IsPaused(renderable->unit))
#else
			if (!GetSceneUnit(renderable->unit)->IsPaused())
#endif
			{
				player->Step(dt * 1000.0f * renderable->animationTimeFactor());
			}
			player->ApplyFrameValues();
			player->ApplyFrameTriggerValues();
		}
	}
*/

	/*
	void DestroyRenderables()
	{
		for (auto& [id, container] : RenderableSUsceneObjects)
		{
			for (auto& [uuid, _] : container)
			{
				RenderableID r = MAKESUUUID(id, uuid);
				DeleteRenderableSceneObject(r);
			}
		}
#include <TrackUUID/JClear.h>
#include "RenderableAtt.h"
#include <JEnd.h>
	}
	*/

	/*
	void DestroyRenderables(SceneUnitId id)
	{
		std::set<JUUID> uuids;
		std::transform(RenderableSUsceneObjects.at(id).begin(), RenderableSUsceneObjects.at(id).end(), std::inserter(uuids, uuids.begin()), [](auto& pair) { return pair.first; });
		for (auto& uuid : uuids)
		{
			DeleteRenderableSceneObject(MAKESUUUID(id, uuid));
		}
#include <TrackUUID/JClearUnit.h>
#include "RenderableAtt.h"
#include <JEnd.h>
	}
	*/

	/*
	void DeleteRenderable(SceneUnitId id, JUUID uuid)
	{
		RenderableID r = MAKESUUUID(id, uuid);
		r->markedForDelete = true;
	}
	*/

	/*
	void RunBoundingBoxComputeShaders(SceneUnitId id)
	{
		using namespace Scene;

		if (!RenderableSUsceneObjects.contains(id)) return;

		auto& scene = GetSceneUnit(id);
		for (auto& [uuid, _] : RenderableSUsceneObjects.at(id))
		{
			RenderableID renderable = MAKESUUUID(id, uuid);

			if (renderable->boundingBoxCompute.empty() ||
				!renderable->boundingBoxCompute->canCompute ||
				!renderable->RenderReady()
				)
				continue;

			renderable->boundingBoxCompute->Compute(id);
		}
	}
	*/

	/*
	void RunBoundingBoxComputeShadersSolution(SceneUnitId id)
	{
		using namespace Scene;

		if (!RenderableSUsceneObjects.contains(id)) return;

		auto& scene = GetSceneUnit(id);
		for (auto& [uuid, _] : RenderableSUsceneObjects.at(id))
		{
			RenderableID renderable = MAKESUUUID(id, uuid);

			if (renderable->boundingBoxCompute.empty() ||
				!renderable->boundingBoxCompute->canCompute ||
				!renderable->RenderReady()
				)
				continue;

			renderable->boundingBoxCompute->Solution(id);
		}
	}
	*/

	/*
#if defined(_EDITOR)
	void WriteRenderablesJson(SceneUnitId id, nlohmann::json& json)
	{
#include <Editor/JSaveFile.h>
#include "RenderableAtt.h"
#include <JEnd.h>
	}
#endif
	*/
}
