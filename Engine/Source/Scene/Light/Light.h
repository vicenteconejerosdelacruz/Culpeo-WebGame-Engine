#pragma once
#include <nlohmann/json.hpp>
#include <Controller.h>
#include <SceneControllerDecl.h>

static constexpr XMVECTOR PointLightDirection[] = {
	{ 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f,-1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f,-1.0f, 0.0f },
};
static constexpr XMVECTOR PointLightUp[] = {
	{ 0.0f, 0.0f,-1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f },
};

inline static const float cascadePartitionsZeroToOne[] = { 0.05f, 0.15f, 0.6f, 1.0f };

enum LightType {
	LT_Ambient,
	LT_Directional,
	LT_Spot,
	LT_Point
};

static std::vector<std::string> LightTypesStr = {
	"Ambient",
	"Directional",
	"Spot",
	"Point"
};

static std::unordered_map<LightType, std::string> LightTypeToString = {
	{ LT_Ambient, "Ambient" },
	{ LT_Directional, "Directional" },
	{ LT_Spot, "Spot" },
	{ LT_Point, "Point" }
};

static std::unordered_map<std::string, LightType> StringToLightType = {
	{ "Ambient", LT_Ambient },
	{ "Directional", LT_Directional },
	{ "Spot", LT_Spot },
	{ "Point",	LT_Point }
};

#if defined(_EDITOR)

static const std::unordered_map<LightType, nlohmann::json> editorDefaultLightProperties = {
	{ LT_Ambient, {
		{ "color", { 0.3f, 0.3f, 0.3f} },
	}},
	{
		LT_Directional, {
		{ "color", { 1.0f, 1.0f, 1.0f } },
		{ "rotation", { -90.0f, 0.0f, 0.0 } },
		{ "shadowMapWidth", 1024 },
		{ "shadowMapHeight", 1024 },
		{ "zBias", 0.0002 },
		{ "hasShadowMaps", false },
	}},
	{	LT_Spot, {
		{ "color", { 1.0f, 1.0f, 1.0f } },
		{ "position", { 0.0f, 10.0f, 0.0f } },
		{ "rotation", { -90.0f, 0.0, 0.0 } },
		{ "coneAngle", 45.0f },
		{ "attenuation" , { 0.0f, 0.001f, 0.0001f } },
		{ "shadowMapWidth", 1024 },
		{ "shadowMapHeight", 1024 },
		{ "zBias", 0.000002 },
		{ "hasShadowMaps", false },
	}},
	{
		LT_Point, {
		{ "color", { 1.0f, 1.0f, 1.0f } },
		{ "position", { 0.0f, 10.0f, 0.0f } },
		{ "attenuation" , { 0.0f, 0.001f, 0.0001f } },
		{ "shadowMapWidth", 1024 },
		{ "shadowMapHeight", 1024 },
		{ "zBias", 0.000002 },
		{ "hasShadowMaps", false },
	}}
};

static inline std::unordered_map<LightType, std::vector<std::string>> shadowMapSizes =
{
	{ LT_Directional, { "32","64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384" }},
	{ LT_Spot, { "32","64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384" }},
	{ LT_Point, { "32","64", "128", "256", "512", "1024", "2048" }}
};

#endif

inline static const unsigned int MaxLights = 100U;
struct LightPool {
    unsigned int numLights;
	unsigned int lightType[MaxLights];
    unsigned int shadowMapIndex[MaxLights];
    unsigned int hasShadowMap[MaxLights];
	unsigned int _pad_padding[3];
    XMFLOAT4 lightColor[MaxLights];
    XMFLOAT4 atts1[MaxLights];
    XMFLOAT4 atts2[MaxLights];
    XMFLOAT4 atts3[MaxLights];
};

inline void DumpLightPool(const LightPool& pool, unsigned int numLights = MaxLights)
{
	static_assert(sizeof(LightPool) == 7616, "LightPool size is not 7616 bytes");
	std::cout << "numLights " << pool.numLights << std::endl;
	for(unsigned int i = 0; i < numLights; i++)
	{
		std::cout << "light(" << i << ")" << std::endl;
		std::cout << "lightType("<< pool.lightType[i] <<") " << LightTypeToString.at(LightType(pool.lightType[i])) << std::endl;
		std::cout << "hasShadowMap " << pool.hasShadowMap[i] << std::endl;
		std::cout << "shadowMapIndex " << pool.shadowMapIndex[i] << std	::endl;
		PrintXMFloat4(pool.lightColor[i],"lightColor");	
		PrintXMFloat4(pool.atts1[i],"atts1");
		PrintXMFloat4(pool.atts2[i],"atts2");
		PrintXMFloat4(pool.atts3[i],"atts3");
	}
}

namespace Scene {

#if defined(_EDITOR)

#include <Attributes/JOrder.h>
#include "LightAtt.h"
#include <JEnd.h>

// #include <Editor/JDrawersDecl.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDecl.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #include <Creator/JRequired.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDecl.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDecl.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDecl.h>
// #include "LightAtt.h"
// #include <JEnd.h>

#endif

	using namespace Game;
	struct Light : Controller
	{
#include <Attributes/JFlags.h>
#include "LightAtt.h"
#include <JEnd.h>

#include <Attributes/JStr2Flag.h>
#include "LightAtt.h"
#include <JEnd.h>

#include <Attributes/JDecl.h>
#include "LightAtt.h"
#include <JEnd.h>

		DEF_STRING2FLAGS_FUNC(Light, Controller);

		//lifecycle
		Light(nlohmann::json& json);
		~Light() { 
			// Destroy();
		}

		void Map(SUUUID so) override;
		void Unmap() override;
#if defined(_EDITOR)
		// void DropJsonMoldAttributes(nlohmann::json& j) override;
#endif

		// void Destroy();

		XMMATRIX world();
		XMVECTOR fw();

		//CREATE
		// void Initialize() override;
		// virtual void SetInitialConditions();
		void BindToScene() override;
		void BindCameras();
		void BindCamera(JUUID cuuid);
		void BindToCamera(JUUID camId) override;
		// void BindRenderablesToShadowMapCamera();
		// virtual void UnbindFromScene();
		// void UnbindCameras();
		// void UnbindCamera(CameraID camera);
		// void UnbindRenderablesFromShadowMapCameras();
		// void UnbindRenderableFromShadowMapCamera(RenderableID r);

		// void LoadShadowMap();
		// void CreateShadowMap();
		// nlohmann::json CreateDirectionalShadowMapCameraJson(unsigned camIndex);
		// nlohmann::json CreateSpotShadowMapCameraJson();
		// nlohmann::json CreatePointShadowMapCameraJson(unsigned camIndex);
		// void CreateDirectionalLightShadowMap();
		// void CreateSpotLightShadowMap();
		// void CreatePointLightShadowMap();
		// void CreateShadowMapDepthStencilResource();
		// void CreateShadowMapShaderResourceView();
		// void UnloadShadowMap();
		// void DestroyShadowMap();
		// void DestroyShadowMapCameras();

		// void UpdateShadowMapCameraProperties();
		// void UpdateDirectionalShadowMapCameraProperties();
		// void CreateDirectionalCascadeShadowMapViewProjectionMatrices();
		// void UpdateSpotShadowMapCameraProperties();
		// void UpdatePointShadowMapCameraProperties();
		// void UpdateShadowMapCameraTransformation();
		// void UpdateDirectionalShadowMapCameraTransformation();
		// void UpdateSpotShadowMapCameraTransformation();
		// void UpdatePointShadowMapCameraTransformation();

		// bool RenderReady();
		// void RenderReady(bool value);

		void WriteUniformBufferLightAttributes(LightPool& lights, unsigned int index);
		// void WriteConstantsBufferShadowMapAttributes(ShadowMapAttributes& atts);
		// void RenderShadowMap(std::function<void(unsigned int)> renderScene);

#if defined(_EDITOR)
		// virtual void EditorPreview(size_t flags);
		// virtual void DestroyEditorPreview();
		// void CreateShadowMapMinMaxChain();
		// void DestroyShadowMapMinMaxChain();
		// void RenderShadowMapMinMaxChain();

		// //Billboard
		RenderableID CreateBillboard(CameraID camera) override;
		// virtual void UpdateBillboard(RenderableID renderable);
		// BoundingBox GetBoundingBox();
		// //Gizmo
		// virtual bool CanInteractWithGizmo(ImGuizmo::OPERATION operation);
		void WriteJson(nlohmann::json& j) override;
		// static void GatherFiles(nlohmann::json& json, std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream);
#endif

		// DeleteHook markedForDelete;
		bool renderReady = false;
		//Transformation
		// XMVECTOR rotationQuaternion;
		//Camera
		unsigned int shadowMapIndex = 0xFFFFFFFF;
		// std::vector<CameraID> shadowMapCameras;
		// std::vector<D3D12_RECT> shadowMapScissorRect;
		// std::vector<D3D12_VIEWPORT> shadowMapViewport;
		// RenderToTexturePassID shadowMapRenderPass;
		// std::vector<std::tuple<float, float>> shadowMapNearFarPlanes;
		// XMFLOAT2 shadowMapTexelInvSize;
#if defined(_EDITOR)
		// unsigned int const destroyStepsCount = 2U;
		// unsigned int destroySteps;
		// bool destroySMChain = false;
		// std::vector<RenderPassInstanceID> shadowMapMinMaxChainRenderPass;
		// RenderPassInstanceID shadowMapMinMaxChainResultRenderPass;
#endif
	};

	CONTROLLER_DECL_FULL(Light);

#include <TrackUUID/JDecl.h>
#include "LightAtt.h"
#include <JEnd.h>

// 	void LightsStep(SceneUnitId unit);
// 	void DestroyLights();
// 	void DestroyLights(SceneUnitId unit);
// 	void DeleteLight(SceneUnitId unit, JUUID uuid);
// #if defined(_EDITOR)
// 	void WriteLightsJson(SceneUnitId id, nlohmann::json& json);
// #endif
};