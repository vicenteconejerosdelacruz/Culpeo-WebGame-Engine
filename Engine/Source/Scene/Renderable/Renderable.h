#pragma once
#include <nlohmann/json.hpp>
#include <Controller.h>
#include <Model3D/Model3D.h>
#include <Material/Material.h>
#include <Material/MeshMaterial.h>
#include <Mesh/Mesh.h>
#include <Camera/Camera.h>
#include <RenderPass/RenderPass.h>
#include <RenderPass/PassMaterialOverride.h>
#include <Sequence/AnimationSequences.h>
#include <Sequence/SequencePlayer.h>
#include <DeviceUtils/UniformBuffer/UniformBuffer.h>
//#include <Renderable/RenderableBoundingBox.h>
// #include <PhysicObject.h>
// #include <NoV8.h>

using RenderableMeshes = std::vector<MeshInstanceID>;
using RenderableMaterials = std::unordered_map<RenderPassInstanceID, std::vector<MaterialInstanceID>>; //RenderPassInstanceID -> MaterialInstanceID
using RenderableUniformBuffer = std::unordered_map<RenderPassInstanceID, std::vector<UniformBufferID>>; //RenderPassInstanceID -> vector<UniformBufferID>[meshIdx]
using RenderableBindGroupLayout = std::unordered_map<RenderPassInstanceID, std::vector<std::vector<wgpu::BindGroupLayout>>>; // RenderPassInstanceID -> vector<vector<BindGroupLayout>>[meshIdx][groupId]
using RenderableBindGroup =  std::unordered_map<RenderPassInstanceID, std::vector<std::vector<std::vector<wgpu::BindGroup>>>>; //RenderPassInstanceID -> vector<vector<vector<BindGroup>>>[meshIdx][groupId][frame]
using RenderableShaderBinding = std::unordered_map<RenderPassInstanceID, std::vector<std::vector<std::vector<ShaderResourceBinding>>>>; //RenderPassInstanceID -> vector<vector<ShaderResourceBinding>>[mesh][groupId][binding]
using RenderableBindingGroupOffsets = std::unordered_map<RenderPassInstanceID, std::unordered_map<unsigned int, size_t>>; //RenderPassInstanceID -> map<group, size_t>
using RenderablePipelines = std::unordered_map<RenderPassInstanceID, std::vector<wgpu::RenderPipeline>>; //RenderPassInstanceID -> vector<pipeline>[mesh]
using BindGroupSetter = std::function<void(wgpu::RenderPassEncoder& encoder, unsigned int backBufferIndex)>;
using BindGroupRender = std::map<std::tuple<CameraID, RenderPassInstanceID>, std::vector<std::vector<BindGroupSetter>>>;
// using DescriptorTableSetter = std::function<void(CComPtr<ID3D12GraphicsCommandList2>& commandList, unsigned int backBufferIndex, unsigned int& slot)>;
// using DescriptorTableRender = std::map<std::tuple<CameraID, RenderPassInstanceID>, std::vector<std::vector<DescriptorTableSetter>>>;

// static nlohmann::json defaultShadowMapShaderAttributes = { { "uniqueMaterialInstance", false }, { "castShadows",false }, { "ibl", false} };
// #if defined(_EDITOR)
// static nlohmann::json defaultPickingShaderAttributes = { { "uniqueMaterialInstance", true}, {"castShadows", false}, {"ibl" , false } };
// #endif

// using namespace Game;
// using namespace Physics;
// using namespace nov8;

namespace Scene
{
#if defined(_EDITOR)

#include <Attributes/JOrder.h>
#include "RenderableAtt.h"
#include <JEnd.h>

// #include <Editor/JDrawersDecl.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDecl.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #include <Creator/JRequired.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDecl.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDecl.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDecl.h>
// #include "RenderableAtt.h"
// #include <JEnd.h>

#endif

	using namespace Game;
	using namespace DeviceUtils;
	using namespace Templates;
	struct Renderable : Controller
	{
		inline static const std::string fallbackMaterialName = "BaseLighting";

#include <Attributes/JFlags.h>
#include "RenderableAtt.h"
#include <JEnd.h>

#include <Attributes/JStr2Flag.h>
#include "RenderableAtt.h"
#include <JEnd.h>

#include <Attributes/JDecl.h>
#include "RenderableAtt.h"
#include <JEnd.h>

		DEF_STRING2FLAGS_FUNC(Renderable, Controller);

#if defined(_EDITOR)
		// static void GatherFiles(nlohmann::json& json, std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream);
#endif

		//lifecycle
		Renderable(nlohmann::json& json);
		~Renderable() {
			// Destroy();
		}
		
		void Map(SUUUID so) override;
		void Unmap() override;
#if defined(_EDITOR)
		// void DropJsonMoldAttributes(nlohmann::json& j) override;
#endif

		XMMATRIX world();

		// void Initialize() override;
		void SetInitialConditions() override;
		void BindToScene() override;
		void Bind(JUUID uuid) override;
		void BindToCamera(JUUID camId) override;
		void BindCameras();
		void BindCamera(JUUID cuuid);
		void BindShadowMapCameras();
		void UnbindFromScene() override;
		void Unbind(JUUID uuid) override;
		void UnbindCameras();
		void UnbindCamera(JUUID cuuid);

		//Render Passes
		std::vector<RenderPassInstanceID> GetCameraRenderPasses(CameraID cam);

		//Meshes
		void CreateMeshInstances();

		//Materials
		void CreateMaterialsInstances(CameraID cam);
		void CreateRenderPassMaterialsInstances(RenderPassInstanceID pass);
		void DestroyMaterialsInstances(CameraID cam);
		void DestroyRenderPassMaterialsInstances(RenderPassInstanceID pass);
		//Pipelines
		void CreatePipelines(CameraID cam);
		void CreateRenderPassPipelines(RenderPassInstanceID pass);
		void DestroyPipelines(CameraID cam);
		void DestroyRenderPassPipelines(RenderPassInstanceID pass);
		//Uniform Buffers
		void CreateUniformBuffersInstances(CameraID cam);
		void CreateRenderPassUniformBuffersInstances(RenderPassInstanceID pass);
		void DestroyUniformBuffersInstances(CameraID cam);
		void DestroyRenderPassUniformBuffersInstances(RenderPassInstanceID pass);
		//Root Signatures
		void CreateBindGroups(CameraID cam);
		void CreateRenderPassBindGroups(RenderPassInstanceID rp);
		void DestroyBindGroups(CameraID cam);
		void DestroyRenderPassBindGroups(RenderPassInstanceID rp);
		//Render Methods
		void CreateRenderMethods(CameraID cam);
		std::vector<std::vector<BindGroupSetter>> CreateRenderPassRenderMethods(CameraID cam, RenderPassInstanceID rp);
#if defined(_EDITOR)
		// std::vector<std::vector<DescriptorTableSetter>> CreateEditorCameraRenderPassRenderMethods(CameraID edCam, CameraID cam, RenderPassInstanceID rp);
#endif
		void CreateBoundingBox();
		bool HasBoundingBoxComputed();
		BoundingBox GetBoundingBox();

		// void WriteMaterialVariablesToConstantsBufferSpace(MaterialInstanceID material, ConstantsBufferID cbvData, unsigned int cbvFrameIndex);
		// void WriteConstantsBuffer(std::string constantName, void* data, unsigned int backbufferIndex, unsigned int slot = 0U, size_t offset = 0ULL);
		void WriteAnimationUniformBuffer(unsigned int frame);
		void WriteUniformBuffer(unsigned int frame);

		//Animation
		void CreateAnimationSequences();
		// void RebuildAnimationSequences();
		void SetCurrentAnimation(SequencePlayer* sequencePlayer);
		void SetCurrentAnimation(std::string anim, float startTime = 0.0f, float timeFactor = 1.0f, bool play = true, bool loop = false);
		void CreateAnimationThread();
		void StepAnimation(double elapsedSeconds);
		std::tuple<XMMATRIX, XMFLOAT3, XMFLOAT3, XMVECTOR, XMFLOAT3> GetBoneTransformation(std::string bone);
		std::vector<std::string> GetBones();

		//DESTROY
		// void Destroy();

		void Render(SceneUnitId unit, wgpu::RenderPassEncoder& pass, RenderPassInstanceID renderPass, CameraID camera);
		// bool RenderReady();
		// void RenderReady(bool value);

		//Scripting
		// std::vector<ScriptBinding> GetScriptBindings() override;
#if defined(_EDITOR)
		// virtual std::map<std::string, ScriptBinding> GetScriptBindingOptions();
#endif

#if defined(_EDITOR)
		// std::function<void()> OnPick;
		//Gizmo
		// virtual bool CanInteractWithGizmo(ImGuizmo::OPERATION operation) { return true; }
		void WriteJson(nlohmann::json& j) override;
#endif

		//State
		// DeleteHook markedForDelete;
		// unsigned int deleteFrames;
		// bool renderReady = false;
		// bool renderException = false;
		//Model3D
		Model3DInstanceID model3D;
		//Meshes
		UniformBufferID worldUb;
		RenderableMeshes meshes;
		RenderableMaterials materials;
		RenderablePipelines pipelines;
		RenderableUniformBuffer uniformBuffers;
		// std::unique_ptr<std::atomic_bool> constantsBuffersLock[JRenderer::numFrames];
		std::unordered_map<std::string, std::vector<UniformBufferBufferWritter>> uniformsWriter;
		// std::unordered_map<std::string, std::vector<ConstantsBufferReader>> constantsReader;
		RenderableShaderBinding shaderBindings;
		RenderableBindGroupLayout bindGroupsLayout;
		RenderableBindGroup bindGroups;
		BindGroupRender bindGroupRenders;
		RenderableBindingGroupOffsets bingGroupOffsets;
		//Animations
		float lastAnimationTime;
		bool forceAnimation;
		std::unique_ptr<std::atomic_bool> animationStepLock;
		std::unique_ptr<std::atomic_bool> animationThreadAlive;
		std::thread animationThread;
		Model3DInstanceID animable;
		Animation::BonesTransformations bonesTransformation;
		Animation::NodeTransformsMap globalNodeTransforms;
		XMMATRIX animationTransformation;
		AnimationSequences animationsSequences;
		SequencePlayer sequencePlayer;
		Animation::BonesTransformations sequenceBoneTransformations;
		//Cameras
		std::set<CameraID> bindedCameras;

		BoundingBox boundingBox;
		//RenderableBoundingBoxID boundingBoxCompute; //used for animables
	};

	CONTROLLER_DECL_FULL(Renderable);
	// SODECL_FULL(Renderable);

#include <TrackUUID/JDecl.h>
#include "RenderableAtt.h"
#include <JEnd.h>

	// void RenderablesStep(SceneUnitId id, float dt);
	// void DestroyRenderables();
	// void DestroyRenderables(SceneUnitId id);
	// void DeleteRenderable(SceneUnitId id, JUUID uuid);
	// //void RunBoundingBoxComputeShaders(SceneUnitId id);
	// //void RunBoundingBoxComputeShadersSolution(SceneUnitId id);
#if defined(_EDITOR)
	// void WriteRenderablesJson(SceneUnitId id, nlohmann::json& json);
#endif
}

// using namespace Scene;
// DEF_SCENEOBJECT_ID_HASH(Renderable);