#pragma once
#include <JObject.h>
#include <JTypes.h>

namespace Primitives
{
	struct Primitive : JObject
	{
		Primitive(nlohmann::json& json) : JObject(json) {}
		virtual void PrepareMesh() {}
#if defined(_EDITOR)
		virtual void WriteJson(nlohmann::json& j) {};
#endif
	};
}