#pragma once
#include <nlohmann/json.hpp>
#include "GraphicsFloat.h"

/*
inline void UpdateSystemCreatedFromJson(unsigned int& flags, nlohmann::json object) {
	flags |= (object.contains("systemCreated") and object["systemCreated"] != "") ? TemplateFlags::SystemCreated : TemplateFlags::None;
}
*/
using namespace Graphics;

inline FLOAT3 JsonToFloat3(nlohmann::json f3) {
	return FLOAT3( f3[0], f3[1], f3[2] );
}

inline void JsonToFloat3(FLOAT3& value, nlohmann::json f3, std::string key) {
	if (f3.contains(key)) { value = FLOAT3(f3[key][0], f3[key][1], f3[key][2]); }
}

template<typename T>
inline void SetIfMissingJson(nlohmann::json& j, std::string key, T value)
{
	if (!j.contains(key)) { j[key] = value; }
}

template<>
inline void SetIfMissingJson<FLOAT3>(nlohmann::json& j, std::string key, FLOAT3 value)
{
	if (!j.contains(key)) { j[key] = nlohmann::json::array({ value.x,value.y,value.z }); }
}

template<>
inline void SetIfMissingJson<FLOAT2>(nlohmann::json& j, std::string key, FLOAT2 value)
{
	if (!j.contains(key)) { j[key] = nlohmann::json::array({ value.x,value.y }); }
}

inline FLOAT2 JsonToFloat2(nlohmann::json f2) {
	return FLOAT2( f2[0], f2[1] );
}

inline void JsonToFloat2(FLOAT2& value, nlohmann::json f2, std::string key) {
	if (f2.contains(key)) { value = FLOAT2( f2[key][0], f2[key][1] ); }
}

template<typename T>
std::set<T> TransformJsonArrayToSet(nlohmann::json j) {
	std::set<T> set;
	for (auto& value : j) {
		T v = value;
		set.insert(v);
	}
	return set;
}

template<typename T>
void TransformJsonArrayToSet(std::set<T>& set, nlohmann::json j, std::string key) {
	if (!j.contains(key)) return;
	for (auto& value : j[key])
	{
		T v = value;
		set.insert(v);
	}
}

template<typename T>
void ReplaceFromJson(T& value, nlohmann::json object, const std::string& key) {
	if (object.contains(key)) value = static_cast<T>(object[key]);
}

inline void RemoveJsonAttributes(nlohmann::json& dst, std::vector<std::string>& atts) {
	for (auto& key : atts)
	{
		if (dst.contains(key)) dst.erase(key);
	}
}

