#include "SceneObject.h"
#include <memory>
#include <mutex>

#if defined(_EDITOR)
namespace Editor
{
	// void MarkSceneUnitAsModified(SceneUnitId id);
};
#endif

namespace Scene
{
	using namespace Graphics;
	typedef std::tuple<JNAME, std::unique_ptr<SceneObject>> SceneObjectTuple;
	typedef SceneObjectsContainer<SceneObjectTuple> SceneObjects;
	std::map<SceneUnitId, SceneObjects> SUSceneObjects;

	SceneObject::SceneObject(SceneUnitId id, nlohmann::json& json) :JObject(json)
	{
		#include <Attributes/JInit.h>
		#include "SceneObjectAtt.h"
		#include <JEnd.h>

		#include <Attributes/JUpdate.h>
		#include "SceneObjectAtt.h"
		#include <JEnd.h>

		// #include <Attributes/JV8Att.h>
		// #include "RenderableAtt.h"
		// #include <JEnd.h>
		unit = id;
		soName = json.at("name");
	}

	void SceneObject::create_rotation(FLOAT3 v)
	{
		if (!contains("rotation"))
		{
			rotation(v);
		}
	}

	void SceneObject::rotation(FLOAT3 v)
	{
		(*this)["rotation"] = FromFLOAT3(v);
		//updateRotationQ();
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
}