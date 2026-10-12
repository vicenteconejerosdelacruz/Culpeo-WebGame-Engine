#include "Variables.h"

using namespace Graphics;

template<>
MaterialVariableInitialValue TransformJsonToMaterialVariableInitialValue<MAT_VAR_RGB, FLOAT3>(nlohmann::json json)
{
	FLOAT3 value = FLOAT3({ json["value"][0] , json["value"][1], json["value"][2] });
	return MaterialVariableInitialValue({ .variableType = MAT_VAR_RGB, .value = value });
}

template<>
MaterialVariableInitialValue TransformJsonToMaterialVariableInitialValue<MAT_VAR_RGBA, FLOAT4>(nlohmann::json json)
{
	FLOAT4 value = FLOAT4({ json["value"][0] , json["value"][1], json["value"][2], json["value"][3] });
	return MaterialVariableInitialValue({ .variableType = MAT_VAR_RGBA, .value = value });
}

template<>
MaterialVariableInitialValue TransformJsonToMaterialVariableInitialValue<MAT_VAR_FLOAT2, FLOAT2>(nlohmann::json json)
{
	FLOAT2 value = FLOAT2({ json["value"][0] , json["value"][1] });
	return MaterialVariableInitialValue({ .variableType = MAT_VAR_FLOAT2, .value = value });
}

template<>
MaterialVariableInitialValue TransformJsonToMaterialVariableInitialValue<MAT_VAR_FLOAT3, FLOAT3>(nlohmann::json json)
{
	FLOAT3 value = FLOAT3({ json["value"][0] , json["value"][1], json["value"][2] });
	return MaterialVariableInitialValue({ .variableType = MAT_VAR_FLOAT3, .value = value });
}

template<>
MaterialVariableInitialValue TransformJsonToMaterialVariableInitialValue<MAT_VAR_FLOAT4, FLOAT4>(nlohmann::json json)
{
	FLOAT4 value = FLOAT4({ json["value"][0] , json["value"][1], json["value"][2], json["value"][3] });
	return MaterialVariableInitialValue({ .variableType = MAT_VAR_FLOAT4, .value = value });
}

/*
template<>
MaterialVariableInitialValue TransformJsonToMaterialVariableInitialValue<MAT_VAR_MATRIX4X4, XMMATRIX>(nlohmann::json json)
{
	XMMATRIX value = XMMATRIX(
		json["value"][0], json["value"][1], json["value"][2], json["value"][3],
		json["value"][4], json["value"][5], json["value"][6], json["value"][7],
		json["value"][8], json["value"][9], json["value"][10], json["value"][11],
		json["value"][12], json["value"][13], json["value"][14], json["value"][15]
	);
	return MaterialVariableInitialValue({ .variableType = MAT_VAR_MATRIX4X4, .value = value });
}
*/

static std::map<std::string, std::function<MaterialVariableInitialValue(nlohmann::json)>> JsonToMaterialVariableInitialValue =
{
	{ "BOOLEAN", TransformJsonToMaterialVariableInitialValue<MAT_VAR_BOOLEAN,bool> },
	{ "INTEGER", TransformJsonToMaterialVariableInitialValue<MAT_VAR_INTEGER,int> },
	{ "UNSIGNED_INTEGER", TransformJsonToMaterialVariableInitialValue<MAT_VAR_UNSIGNED_INTEGER,unsigned int> },
	{ "RGB", TransformJsonToMaterialVariableInitialValue<MAT_VAR_RGB,FLOAT3> },
	{ "RGBA", TransformJsonToMaterialVariableInitialValue<MAT_VAR_RGBA,FLOAT4> },
	{ "FLOAT", TransformJsonToMaterialVariableInitialValue<MAT_VAR_FLOAT,float> },
	{ "FLOAT2", TransformJsonToMaterialVariableInitialValue<MAT_VAR_FLOAT2,FLOAT2> },
	{ "FLOAT3", TransformJsonToMaterialVariableInitialValue<MAT_VAR_FLOAT3,FLOAT3> },
	{ "FLOAT4", TransformJsonToMaterialVariableInitialValue<MAT_VAR_FLOAT4,FLOAT4> },
	//{ "MATRIX4X4", TransformJsonToMaterialVariableInitialValue<MAT_VAR_MATRIX4X4,XMMATRIX> }
};

MaterialVariableInitialValue JsonToMaterialInitialValue(nlohmann::json json)
{
	return JsonToMaterialVariableInitialValue.at(json.at("variableType"))(json);
}

static std::map<MaterialVariablesTypes, std::function<void(MaterialVariableInitialValue& def, void* dst, size_t size)>> materialVariableInitialValueToDestionation =
{
	{ MAT_VAR_BOOLEAN, WriteMappedValueToDestination<bool>	},
	{ MAT_VAR_INTEGER, WriteMappedValueToDestination<int> },
	{ MAT_VAR_UNSIGNED_INTEGER,	WriteMappedValueToDestination<unsigned int> },
	{ MAT_VAR_RGB, WriteMappedValueToDestination<FLOAT3> },
	{ MAT_VAR_RGBA,	WriteMappedValueToDestination<FLOAT4> },
	{ MAT_VAR_FLOAT, WriteMappedValueToDestination<float> },
	{ MAT_VAR_FLOAT2,	WriteMappedValueToDestination<FLOAT2> },
	{ MAT_VAR_FLOAT3,	WriteMappedValueToDestination<FLOAT3> },
	{ MAT_VAR_FLOAT4,	WriteMappedValueToDestination<FLOAT4> },
	//{ MAT_VAR_MATRIX4X4, WriteMappedValueToDestination<XMMATRIX> },
};

void WriteMappedInitialValuesToDestination(MaterialVariableInitialValue& def, void* dst, size_t size)
{
	materialVariableInitialValueToDestionation.at(def.variableType)(def, dst, size);
}

template<>
void WriteMaterialVariableInitialValueToJson<FLOAT2>(nlohmann::json& matInitialValue, MaterialVariableInitialValue& varValue)
{
	FLOAT2 value = std::any_cast<FLOAT2>(varValue.value);
	matInitialValue["value"] = { value.x, value.y };
}

template<>
void WriteMaterialVariableInitialValueToJson<FLOAT3>(nlohmann::json& matInitialValue, MaterialVariableInitialValue& varValue)
{
	FLOAT3 value = std::any_cast<FLOAT3>(varValue.value);
	matInitialValue["value"] = { value.x, value.y, value.z };
}

template<>
void WriteMaterialVariableInitialValueToJson<FLOAT4>(nlohmann::json& matInitialValue, MaterialVariableInitialValue& varValue)
{
	FLOAT4 value = std::any_cast<FLOAT4>(varValue.value);
	matInitialValue["value"] = { value.x, value.y, value.z };
}

// template<>
// void WriteMaterialVariableInitialValueToJson<XMMATRIX>(nlohmann::json& matInitialValue, MaterialVariableInitialValue& varValue)
// {
// 	XMMATRIX value = std::any_cast<XMMATRIX>(varValue.value);
// 	matInitialValue["value"] = {
// 		value.r[0].m128_f32[0], value.r[0].m128_f32[1], value.r[0].m128_f32[2], value.r[0].m128_f32[3],
// 		value.r[1].m128_f32[0], value.r[1].m128_f32[1], value.r[1].m128_f32[2], value.r[1].m128_f32[3],
// 		value.r[2].m128_f32[0], value.r[2].m128_f32[1], value.r[2].m128_f32[2], value.r[2].m128_f32[3],
// 		value.r[3].m128_f32[0], value.r[3].m128_f32[1], value.r[3].m128_f32[2], value.r[3].m128_f32[3],
// 	};
// }

void valueMappingToJson(MaterialVariablesTypes type, nlohmann::json& matInitialValue, MaterialVariableInitialValue& varValue)
{
	const std::map<MaterialVariablesTypes, std::function<void(nlohmann::json& matInitialValue, MaterialVariableInitialValue& varValue)>> vMappingToJson = {
		{ MAT_VAR_BOOLEAN, WriteMaterialVariableInitialValueToJson<bool>},
		{ MAT_VAR_INTEGER, WriteMaterialVariableInitialValueToJson<int>},
		{ MAT_VAR_UNSIGNED_INTEGER, WriteMaterialVariableInitialValueToJson<unsigned int>},
		{ MAT_VAR_RGB, WriteMaterialVariableInitialValueToJson<FLOAT3>},
		{ MAT_VAR_RGBA, WriteMaterialVariableInitialValueToJson<FLOAT4>},
		{ MAT_VAR_FLOAT, WriteMaterialVariableInitialValueToJson<float>},
		{ MAT_VAR_FLOAT2, WriteMaterialVariableInitialValueToJson<FLOAT2>},
		{ MAT_VAR_FLOAT3, WriteMaterialVariableInitialValueToJson<FLOAT3>},
		{ MAT_VAR_FLOAT4, WriteMaterialVariableInitialValueToJson<FLOAT4>},
		// { MAT_VAR_MATRIX4X4, WriteMaterialVariableInitialValueToJson<XMMATRIX>},
	};
	return vMappingToJson.at(type)(matInitialValue, varValue);
}

nlohmann::json TransformMaterialValueMappingToJson(MaterialInitialValueMap mappedValues) {

	//using namespace DirectX;
	nlohmann::json jMappedValues = nlohmann::json::array();

	std::transform(mappedValues.begin(), mappedValues.end(), std::back_inserter(jMappedValues), [](MaterialInitialValuePair pair)
		{
			nlohmann::json matInitialValue = nlohmann::json({});

			matInitialValue["variable"] = pair.first;
			matInitialValue["variableType"] = MaterialVariablesTypesToString.at(pair.second.variableType);
			valueMappingToJson(pair.second.variableType, matInitialValue, pair.second);
			return matInitialValue;
		}
	);

	return jMappedValues;
}

void TransformJsonToMaterialValueMapping(MaterialInitialValueMap& map, nlohmann::json object, std::string key) {

	// using namespace DirectX;

	if (!object.contains(key)) return;

	nlohmann::json mappedValues = object[key];

	std::transform(mappedValues.begin(), mappedValues.end(), std::inserter(map, map.end()), [](nlohmann::json value)
		{
			MaterialInitialValuePair pair;
			pair.first = value["variable"];
			pair.second = JsonToMaterialInitialValue(value);
			return pair;
		}
	);
}


