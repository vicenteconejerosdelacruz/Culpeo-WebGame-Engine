#include "SceneObject.h"
#include <memory>
#include <mutex>
#include <SimpleMath.h>

#if defined(_EDITOR)
namespace Editor
{
	// void MarkSceneUnitAsModified(SceneUnitId id);
};
#endif

namespace Scene
{
	//using namespace Graphics;
	using namespace DirectX;
	typedef std::tuple<JNAME, std::unique_ptr<SceneObject>> SceneObjectTuple;
	typedef SceneObjectsContainer<SceneObjectTuple> SceneObjects;
	std::map<SceneUnitId, SceneObjects> SUSceneObjects;

	SceneObject::SceneObject(SceneUnitId id, nlohmann::json& json) :JObject(json)
	{
		unit = id;
		soName = json.at("name");
		
		#include <Attributes/JInit.h>
		#include "SceneObjectAtt.h"
		#include <JEnd.h>

		#include <Attributes/JUpdate.h>
		#include "SceneObjectAtt.h"
		#include <JEnd.h>

		// #include <Attributes/JV8Att.h>
		// #include "RenderableAtt.h"
		// #include <JEnd.h>
	}

	void SceneObject::create_rotation(XMFLOAT3 v)
	{
		if (!contains("rotation"))
		{
			rotation(v);
		}
	}

	void SceneObject::rotation(XMFLOAT3 v)
	{
		(*this)["rotation"] = FromXMFLOAT3(v);
		updateRotationQ();
	}

	void SceneObject::Initialize()
	{
	}

	void SceneObject::JUpdate(nlohmann::json p)
	{
// #if defined(_EDITOR)
// 		Editor::MarkSceneUnitAsModified(unit);
// #endif
		JObject::JUpdate(p);
	}

	void SceneObject::JPatch(nlohmann::json p)
	{
// #if defined(_EDITOR)
// 		Editor::MarkSceneUnitAsModified(unit);
// #endif
		JObject::JPatch(p);
	}

	void SceneObject::Destroy()
	{
		// for (auto& cb : destroyCallbacks)
		// {
		// 	cb();
		// }
	}

	XMVECTOR SceneObject::rotationQ()
	{
		return rotationQuaternion;
	}

	void SceneObject::rotationQ(XMVECTOR Q)
	{
		rotationQuaternion = Q;
	}

	void SceneObject::updateRotationQ()
	{
		XMFLOAT3 v = rotation();
		rotationQuaternion = XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(v.x),
			XMConvertToRadians(v.y),
			XMConvertToRadians(v.z)
		);
	}

	XMVECTOR SceneObject::positionV()
	{
		XMFLOAT3 pos = position();
		return XMLoadFloat3(&pos);
	}

	XMVECTOR SceneObject::scaleV()
	{
		XMFLOAT3 pos = scale();
		return XMLoadFloat3(&pos);
	}

// #if defined(_EDITOR)
// 	std::map<std::string, ScriptBinding> SceneObject::GetScriptBindingOptions()
// 	{
// 		return {
// 			{ at("name"), ScriptBinding(std::string(at("uuid"))) }
// 		};
// 	}
// #endif

	static inline std::mutex sceneObjectMutex;
	void CreateSceneObject(SceneUnitId id, nlohmann::json& json)
	{
		std::lock_guard<std::mutex> lock(sceneObjectMutex);
		JUUID uuid = json.at("uuid");
		JNAME name = json.at("name");

		if(!SUSceneObjects.contains(id))
			SUSceneObjects[id] = SceneObjects();

		auto& uuidSceneObjects = SUSceneObjects.at(id);
		if (uuidSceneObjects.contains(uuid))
		{
			assert(!!!"creation collision");
		}

		//build
		std::unique_ptr<SceneObject> jT = std::make_unique<SceneObject>(id, json);
		uuidSceneObjects.insert_or_assign(uuid, std::make_tuple(name, std::move(jT)));
		std::get<1>(uuidSceneObjects.at(uuid))->Initialize();
	}

	std::unique_ptr<SceneObject>& GetSceneObject(SceneUnitId id, JUUID uuid)
	{
		auto& uuidSceneObjects = SUSceneObjects.at(id);
		return std::get<1>(uuidSceneObjects.at(uuid));
	}
}