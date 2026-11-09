#include "Light.h"
#include <SceneObject.h>
#include <Scene.h>
#if defined(_EDITOR)
#include <Editor.h>
// #include <Builder/ReleaseBuilder.h>
#endif
#include <SimpleMath.h>

namespace Scene
{
#include <TrackUUID/JDef.h>
#include "LightAtt.h"
#include <JEnd.h>

// #if defined(_EDITOR)

// #include <Editor/JDrawersDef.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDef.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDef.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDef.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDef.h>
// #include "LightAtt.h"
// #include <JEnd.h>

// #endif

	// using namespace DeviceUtils;

	Light::Light(nlohmann::json& json) : Controller(json)
	{
#include <Attributes/JInit.h>
#include "LightAtt.h"
#include <JEnd.h>

#include <Attributes/JUpdate.h>
#include "LightAtt.h"
#include <JEnd.h>

// #include <Attributes/JV8Att.h>
// #include "LightAtt.h"
// #include <JEnd.h>
// 		RENAME_ON_DELETION(Light);
	}

	void Light::Map(SUUUID so)
	{
		#if defined(_EDITOR)
		using namespace Editor;
		#endif
		
		Controller::Map(so);
		#include <TrackUUID/JInsert.h>
		#include "LightAtt.h"
		#include <JEnd.h>

		#if defined(_EDITOR)
		if (lightType() != LT_Ambient && !SceneIsIsolated(unit))
			RegisterBillboard(uuid());
		#endif
	}

	void Light::Unmap()
	{
		#include <TrackUUID/JErase.h>
		#include "LightAtt.h"
		#include <JEnd.h>
		Controller::Unmap();
	}

#if defined(_EDITOR)
	void Light::WriteJson(nlohmann::json& j)
	{
		#include <Editor/JWriteJson.h>
		#include "LightAtt.h"
		#include <JEnd.h>
		Controller::WriteJson(j);
	}
// 	void Light::GatherFiles(nlohmann::json& json, std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream)
// 	{
// #include <Editor/JReleaseBuilder.h>
// #include "LightAtt.h"
// #include <JEnd.h>
// 	}
#endif

// 	void Light::Initialize()
// 	{
// #if defined(_EDITOR)
// 		using namespace Editor;
// #endif
//
// #include <TrackUUID/JInsert.h>
// #include "LightAtt.h"
// #include <JEnd.h>
//
// 		if (hasShadowMaps())
// 		{
// 			CreateShadowMap();
// 		}
// #if defined(_EDITOR)
// 		if (lightType() != LT_Ambient && !SceneIsIsolated(unit))
// 		{
// 			RegisterBillboard(unit, uuid());
// 		}
// #endif
// 		SetInitialConditions();
// 		SceneObject::Initialize();
// 	}

// 	void Light::SetInitialConditions()
// 	{
// 		updateRotationQ();
// 	}

	void Light::BindToScene()
	{
#include <TrackUUID/JInsert.h>
#include "LightAtt.h"
#include <JEnd.h>

		BindCameras();
		// BindRenderablesToShadowMapCamera();
#if defined(_EDITOR)
		Controller::BindToScene();
#endif
	}

	void Light::BindCameras()
	{
		auto cams = cameras();
		for (auto& suuid : cams)
		{
			SceneObjectID cam(unit,suuid);
			JUUID uuid = cam->controllers_get("camera");
			if (uuid.empty())
				continue;
			BindCamera(uuid);
		}
	}

	void Light::BindCamera(JUUID cuuid)
	{
		Scene::BindToScene(unit, uuid(), cuuid);
	}

	void Light::BindToCamera(JUUID camId)
	{
		CameraID cam(camId);
		cam->BindLight(uuid());
	}
//
// 	void Light::UnbindFromScene()
// 	{
// #include <TrackUUID/JErase.h>
// #include "LightAtt.h"
// #include <JEnd.h>
//
// 		UnbindCameras();
//
// 		if (!hasShadowMaps()) return;
//
// #if defined(_EDITOR)
// 		DestroyShadowMapMinMaxChain();
// #endif
// 		DestroyShadowMap();
// 	}

// 	void Light::UnbindCameras()
// 	{
// 		auto cams = cameras();
// 		for (auto& uuid : cams) {
// 			UnbindCamera(MAKESUUUID(unit, uuid));
// 		}
// 	}

// 	void Light::UnbindCamera(CameraID camera)
// 	{
// 		if (camera.empty()) return;
// 		camera->UnbindLight(MAKESUUUID(unit, uuid()));
// 		Scene::UnbindFromScene(unit, uuid(), camera.uuid());
// 	}

// #if defined(_EDITOR)
// 	void Light::DropJsonMoldAttributes(nlohmann::json& j)
// 	{
// 		SceneObject::DropJsonMoldAttributes(j);
// 		j.at("cameras") = nlohmann::json::array({});
// 	}
// #endif

// 	void Light::Destroy()
// 	{
// #if defined(_EDITOR)
// 		DestroyEditorPreview();
// #endif
// #include <Attributes/JDestroy.h>
// #include "LightAtt.h"
// #include <JEnd.h>
//
// 		SceneObject::Destroy();
// 	}

	XMMATRIX Light::world()
	{
		XMMATRIX rotationM = XMMatrixRotationQuaternion(sceneObject->rotationQ());
		XMMATRIX positionM = XMMatrixTranslationFromVector(sceneObject->positionV());
		return XMMatrixMultiply(rotationM, positionM);
	}

	XMVECTOR Light::fw()
	{
		FXMVECTOR dir = { 0.0f, 0.0f, 1.0f,0.0f };
		XMVECTOR fw = XMVector3Normalize(XMVector3Rotate(dir, sceneObject->rotationQ()));
		return fw;
	}

	// bool Light::RenderReady()
	// {
	// 	return renderReady;
	// }

	// void Light::RenderReady(bool value)
	// {
	// 	for (auto& c : shadowMapCameras)
	// 	{
	// 		c->RenderReady(value);
	// 	}
	// 	renderReady = value;
	// }

	void Light::WriteUniformBufferLightAttributes(LightPool& lights, unsigned int index)
	{
		lights.lightType[index] = static_cast<unsigned int>(lightType());

		switch (lightType())
		{
		case LT_Ambient:
		{
			XMFLOAT3 col = color();
			float b = brightness();
			lights.lightColor[index] = XMFLOAT4(col.x * b, col.y * b, col.z * b, 0.0f);
		}
		break;
		case LT_Directional:
		{
			XMFLOAT3 col = color();
			float b = brightness();
			lights.lightColor[index] = XMFLOAT4(col.x * b, col.y * b, col.z * b, 0.0f);
			XMVECTOR lightDir = fw();
			lights.atts1[index] = *((XMFLOAT4*)&lightDir.vector4_f32[0]);
			lights.hasShadowMap[index] = hasShadowMaps();
			lights.shadowMapIndex[index] = shadowMapIndex;
		}
		break;
		case LT_Spot:
		{
			XMFLOAT3 pos = sceneObject->position();
			XMFLOAT3 atte = attenuation();
			XMFLOAT3 col = color();
			float b = brightness();
			lights.lightColor[index] = XMFLOAT4(col.x * b, col.y * b, col.z * b, 0.0f);
			lights.atts1[index] = { pos.x, pos.y, pos.z, 0.0f };
			XMVECTOR lightDir = fw();
			lights.atts2[index] = *((XMFLOAT4*)&lightDir.vector4_f32[0]);
			lights.atts2[index].w = cosf(XMConvertToRadians(coneAngle()));
			lights.atts3[index] = { atte.x, atte.y, atte.z, 0.0f };
			lights.hasShadowMap[index] = hasShadowMaps();
			lights.shadowMapIndex[index] = shadowMapIndex;
		}
		break;
		case LT_Point:
		{
			XMFLOAT3 pos = sceneObject->position();
			XMFLOAT3 atte = attenuation();
			XMFLOAT3 col = color();
			float b = brightness();
			lights.lightColor[index] = XMFLOAT4(col.x * b, col.y * b, col.z * b, 0.0f);
			lights.atts1[index] = { pos.x, pos.y, pos.z, 0.0f };
			lights.atts2[index] = { atte.x, atte.y, atte.z, 0.0f };
			lights.hasShadowMap[index] = hasShadowMaps();
			lights.shadowMapIndex[index] = shadowMapIndex;
		}
		break;
		}
	}

#if defined(_EDITOR)
/*
	static std::map<LightType, nlohmann::json> defaultShadowMapParameters = {
		{ LT_Directional, {{ "shadowMapWidth",1024}, {"shadowMapHeight",1024}, {"viewWidth", 32.0f}, {"viewHeight",32.0f},{"nearZ",0.01f}, {"farZ",1000.0f}}},
		{ LT_Spot, {{ "shadowMapWidth",1024}, {"shadowMapHeight",1024}, {"viewWidth", 32.0f}, {"viewHeight",32.0f},{"nearZ",0.01f}, {"farZ",100.0f}} },
		{ LT_Point, {{ "shadowMapWidth",1024}, {"shadowMapHeight",1024}, {"nearZ",0.01f}, {"farZ",20.0f}} },
	};

	static std::vector<std::string> shadowMapJsonAttributes = {
		"shadowMapWidth", "shadowMapHeight", "viewWidth", "viewHeight", "nearZ", "farZ",
	};

	void WriteLightsJson(SceneUnitId id, nlohmann::json& json)
	{
#include <Editor/JSaveFile.h>
#include "LightAtt.h"
#include <JEnd.h>
	}

	void Light::EditorPreview(size_t flags)
	{
		if (flags & (1 << Light::Update_hasShadowMaps))
		{
			if (hasShadowMaps())
				CreateShadowMapMinMaxChain();
		}
		switch (lightType())
		{
		case LT_Directional:
		case LT_Spot:
		case LT_Point:
		{
			//leave commented until i know why this was here initialy, not actual part of the convertion comments
			//Editor::SelectSceneObject(uuid());
		}
		break;
		}
	}

	void Light::DestroyEditorPreview()
	{
		destroySMChain = true;
		destroySteps = destroyStepsCount;
		switch (lightType())
		{
		case LT_Directional:
		{
			//leave commented until i know why this was here initialy, not actual part of the convertion comments
			//Editor::DeselectSceneObject(uuid());
		}
		break;
		}
	}
	*/
	RenderableID Light::CreateBillboard(CameraID camera)
	{
		using namespace Editor;

		if (lightType() == LT_Ambient) return RenderableID();

		RenderableID bb = Editor::CreateBillboardFromMaterials(unit, camera, sceneObject->at("name"), "LightBulb", "LightBulbPicking");
		bb->sceneObject->position(sceneObject->position());
		// bb->OnPick = [&] {Editor::SelectLight(SUuuid()); };
		return bb;
	}
	/*

	void Light::UpdateBillboard(RenderableID renderable)
	{
		assert(!renderable.empty());
		if (renderable.empty()) return;

		auto& scene = GetSceneUnit(unit);

		XMFLOAT3 baseColor = color();
		renderable->position(position());
		renderable->WriteConstantsBuffer("baseColor", &baseColor, scene->Frame());
		renderable->WriteConstantsBuffer(scene->Frame());
	}

	BoundingBox Light::GetBoundingBox()
	{
		return BoundingBox(position(), { 0.1f,0.1f,0.1f });
	}

	bool Light::CanInteractWithGizmo(ImGuizmo::OPERATION operation)
	{
		return lightType() != LT_Ambient;
	}
*/
#endif

	/*
	void LightsStep(SceneUnitId id)
	{
#if defined(_DEVELOPMENT)
		std::string event = std::string(__FUNCTION__) + ":" + std::to_string(id);
		PIXScopedEvent(0, nostd::StringToWString(event).c_str());
#endif
		std::set<LightID> lightsToUpdateCamAttributes;
		std::set<LightID> lightsToUpdateTransformation;
		std::set<LightID> lightsToDelete;
		std::set<LightID> lightToRecreateCameras;

		std::set<Light::Light_UpdateFlags> smCamAttributes =
		{
			Light::Update_coneAngle, Light::Update_shadowMapWidth, Light::Update_shadowMapHeight,
			Light::Update_viewWidth, Light::Update_viewHeight, Light::Update_nearZ, Light::Update_farZ
		};
		std::set<Light::Light_UpdateFlags> smCamTransformations =
		{
			Light::Update_position, Light::Update_rotation, Light::Update_dirDist
		};

		auto lights = GetLights(id);

		auto& scene = GetSceneUnit(id);

		for (auto& uuid : lights)
		{
			LightID l = MAKESUUUID(id, uuid);

			//is this(hack) or fix the loading system
			//if (!l->RenderReady() && scene->IsBound(uuid))
			//{
			//	l->RenderReady(true);
			//	scene->EraseLightFromLoadingPool(l);
			//}

			if (l->lightType() != LT_Ambient)
			{
				if (l->dirty(Light::Update_cameras))
				{
					l->RenderReady(false);
					lightToRecreateCameras.insert(l);
				}
			}

#if defined(_EDITOR)
			//if the light type changed
			if (l->dirty(Light::Update_lightType))
			{
				//use default attributes depending of the light type
				l->JUpdate(editorDefaultLightProperties.at(l->lightType()));

				//we deactivate shadowmaps always a light type is converted
				if (l->hasShadowMaps())
				{
					l->hasShadowMaps(false);
				}

				l->clean(Light::Update_lightType);
			}

			if (l->dirty(Light::Update_hasShadowMaps))
			{
				if (l->hasShadowMaps())
				{
					l->LoadShadowMap();
				}
				else
				{
					l->UnloadShadowMap();
				}
				l->clean(Light::Update_hasShadowMaps);
			}

			//if destroying SMChain
			if (l->destroySMChain)
			{
				l->destroySteps--;
				if (l->destroySteps == 0)
				{
					l->destroySMChain = false;
					l->DestroyShadowMapMinMaxChain();
				}
			}
#endif

			if (std::any_of(smCamAttributes.begin(), smCamAttributes.end(), [&l](auto flag) { return l->dirty(flag); }))
			{
				lightsToUpdateCamAttributes.insert(l);
				std::for_each(smCamAttributes.begin(), smCamAttributes.end(), [&l](auto flag) { l->clean(flag); });
			}
			if (std::any_of(smCamTransformations.begin(), smCamTransformations.end(), [&l](auto flag) { return l->dirty(flag); }))
			{
				lightsToUpdateTransformation.insert(l);
				std::for_each(smCamTransformations.begin(), smCamTransformations.end(), [&l](auto flag) { l->clean(flag); });
			}

			if (l->markedForDelete)
			{
				lightsToDelete.insert(l);
			}
			for (auto l : lightsToUpdateCamAttributes)
			{
				l->UpdateShadowMapCameraProperties();
			}
			for (auto l : lightsToUpdateTransformation)
			{
				l->updateRotationQ();
				l->UpdateShadowMapCameraTransformation();
			}
		}

		for (auto l : lightsToDelete)
		{
			EraseLightFromLights(l->unit, l.uuid());
			EraseLightFromShadowMapLights(l->unit, l.uuid());
			DeleteLightSceneObject(l);
		}

		if (lightToRecreateCameras.size() > 0)
		{
			auto& scene = GetSceneUnit(id);
		}
	}

	void DestroyLights()
	{
		for (auto& [id, container] : LightSUsceneObjects)
		{
			for (auto& [uuid, _] : container)
			{
				LightID l = MAKESUUUID(id, uuid);
				DeleteLightSceneObject(l);
			}
		}
#include <TrackUUID/JClear.h>
#include "LightAtt.h"
#include <JEnd.h>
	}

	void DestroyLights(SceneUnitId id)
	{
		std::set<JUUID> uuids;
		std::transform(LightSUsceneObjects.at(id).begin(), LightSUsceneObjects.at(id).end(), std::inserter(uuids, uuids.begin()), [](auto& pair) { return pair.first; });
		for (auto& uuid : uuids)
		{
			DeleteLightSceneObject(MAKESUUUID(id, uuid));
		}
#include <TrackUUID/JClearUnit.h>
#include "LightAtt.h"
#include <JEnd.h>
	}

	void DeleteLight(SceneUnitId id, JUUID uuid)
	{
#if defined(_EDITOR)
		using namespace Editor;
#endif
		LightID l = MAKESUUUID(id, uuid);
#if defined(_EDITOR)
		DestroyBillboard(l->unit, uuid);
#endif
		l->markedForDelete = true;
	}
	*/
}