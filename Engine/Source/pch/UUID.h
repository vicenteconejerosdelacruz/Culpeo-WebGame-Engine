#pragma once

#include <random>
#include <sstream>
#include <iomanip>
#include <string>
#include <tuple>
#include <nlohmann/json.hpp>
#include "SceneUnitId.h"

using JUUID = std::string;
using JNAME = std::string;
using JUUIDName = std::tuple<JUUID, JNAME>;
using SUUUID = std::tuple<SceneUnitId, JUUID>;

#include "NoStd.h"

#define MAKESUUUID(id,juuid) std::make_tuple(id,juuid)
#define FROMSUUUID(tup) std::get<0>(tup),std::get<1>(tup)
#define SUUUIDSTR(tup) std::string(std::to_string(std::get<0>(tup))+"/"+std::get<1>(tup))
#define SUUUIDUNIT(tup) std::get<0>(tup)
#define SUUUIDUUID(tup) std::get<1>(tup)

//Templates
template<typename T, std::unique_ptr<T>& F(JUUID)>
struct TUUID {
	JUUID uuid;
	TUUID() {}
	TUUID(const TUUID& other)
	{
		uuid = other.uuid;
	}
	TUUID(const JUUID& nuuid)
	{
		uuid = nuuid;
	}
	TUUID operator=(TUUID other)
	{
		uuid = other.uuid;
		return *this;
	}
	TUUID operator=(JUUID nuuid)
	{
		uuid = nuuid;
		return *this;
	}
	bool operator==(const TUUID& other) const
	{
		return other.uuid == uuid;
	}
	std::unique_ptr<T>& operator->()
	{
		return F(uuid);
	}
	std::unique_ptr<T>& operator->() const
	{
		return F(uuid);
	}
	std::unique_ptr<T>& operator*()
	{
		return F(uuid);
	}
	bool operator<(const TUUID& other) const
	{
		return uuid < other.uuid;
	}
	JUUID operator()() const
	{
		return uuid;
	}
	JUUID operator()()
	{
		return uuid;
	}
	bool operator!() const
	{
		return empty();
	}
	bool operator!()
	{
		return empty();
	}
	explicit operator bool() const
	{
		return !empty();
	}
	explicit operator bool()
	{
		return !empty();
	}
	JUUID operator+()
	{
		return uuid;
	}
	bool empty() const
	{
		return uuid.empty();
	}
	bool empty()
	{
		return uuid.empty();
	}
	void clear()
	{
		uuid = "";
	}
	void replace(JUUID u)
	{
		uuid = u;
	}
};

#define DEF_TEMPLATE_ID(TYPE,GETTER)\
using TYPE##ID = TUUID<TYPE, GETTER>;

#define DEF_TEMPLATE_ID_DEP(TYPE,GETTER)\
struct TYPE;\
extern std::unique_ptr<TYPE>& GETTER(JUUID uuid);\
DEF_TEMPLATE_ID(TYPE,GETTER)

#define DEF_TEMPLATE_ID_HASH(TYPE)\
template <>\
struct std::hash<TYPE##ID>\
{\
	std::size_t operator()(const TYPE##ID& other) const\
	{\
		return std::hash<std::string>{}(other.uuid);\
	}\
}

//Controllers
template<typename T, T* F(JUUID)>
struct CUUID {
	JUUID uuid;
	CUUID() {}
	CUUID(const CUUID& other)
	{
		uuid = other.uuid;
	}
	CUUID(const JUUID& nuuid)
	{
		uuid = nuuid;
	}
	CUUID operator=(CUUID other)
	{
		uuid = other.uuid;
		return *this;
	}
	CUUID operator=(JUUID nuuid)
	{
		uuid = nuuid;
		return *this;
	}
	bool operator==(const CUUID& other) const
	{
		return other.uuid == uuid;
	}
	T* operator->()
	{
		return F(uuid);
	}
	T* operator->() const
	{
		return F(uuid);
	}
	T* operator*()
	{
		return F(uuid);
	}
	bool operator<(const CUUID& other) const
	{
		return uuid < other.uuid;
	}
	JUUID operator()() const
	{
		return uuid;
	}
	JUUID operator()()
	{
		return uuid;
	}
	bool operator!() const
	{
		return empty();
	}
	bool operator!()
	{
		return empty();
	}
	explicit operator bool() const
	{
		return !empty();
	}
	explicit operator bool()
	{
		return !empty();
	}
	JUUID operator+()
	{
		return uuid;
	}
	bool empty() const
	{
		return uuid.empty();
	}
	bool empty()
	{
		return uuid.empty();
	}
	void clear()
	{
		uuid = "";
	}
	void replace(JUUID u)
	{
		uuid = u;
	}
};

#define DEF_CONTROLLER_ID(TYPE,GETTER)\
using TYPE##ID = CUUID<TYPE, GETTER>;

#define DEF_CONTROLLER_ID_DEP(TYPE_NAMESPACE,TYPE,GETTER)\
namespace TYPE_NAMESPACE\
{\
	struct TYPE;\
	using namespace Game;\
	using TYPE##ID = CUUID<TYPE, GETTER>;\
};\
using namespace TYPE_NAMESPACE;

#define DEF_CONTROLLER_ID_HASH(TYPE)\
template <>\
struct std::hash<TYPE##ID>\
{\
	std::size_t operator()(const TYPE##ID& other) const\
	{\
		return std::hash<std::string>{}(other.uuid);\
	}\
}

//SceneObject
template<typename T, std::unique_ptr<T>& F(SceneUnitId, JUUID)>
struct TSUUUID {
	SUUUID SUuuid;
	std::function<bool()> validator = nullptr;
	TSUUUID() {}
	TSUUUID(const SceneUnitId unit, JUUID uuid)
	{
		SUuuid = MAKESUUUID(unit, uuid);
	}
	TSUUUID(const TSUUUID& other)
	{
		SUuuid = other.SUuuid;
		validator = other.validator;
	}
	TSUUUID(const TSUUUID& other, std::function<bool()> validator)
	{
		SUuuid = other.SUuuid;
		this->validator = validator;
	}
	TSUUUID(const SUUUID& nSUuuid)
	{
		SUuuid = nSUuuid;
	}
	TSUUUID(const SUUUID& nSUuuid, std::function<bool()> validator)
	{
		SUuuid = nSUuuid;
		this->validator = validator;
	}
	TSUUUID operator=(TSUUUID other)
	{
		SUuuid = other.SUuuid;
		validator = other.validator;
		return *this;
	}
	TSUUUID operator=(SUUUID nSUuuid)
	{
		SUuuid = nSUuuid;
		return *this;
	}
	bool operator==(const TSUUUID& other) const
	{
		return (std::get<0>(other.SUuuid) == std::get<0>(SUuuid)) && (std::get<1>(other.SUuuid) == std::get<1>(SUuuid));
	}
	std::unique_ptr<T>& operator->()
	{
		return F(std::get<0>(SUuuid), std::get<1>(SUuuid));
	}
	std::unique_ptr<T>& operator->() const
	{
		return F(std::get<0>(SUuuid), std::get<1>(SUuuid));
	}
	std::unique_ptr<T>& operator*()
	{
		return F(std::get<0>(SUuuid), std::get<1>(SUuuid));
	}
	std::unique_ptr<T>& operator*() const
	{
		return F(std::get<0>(SUuuid), std::get<1>(SUuuid));
	}
	bool operator<(const TSUUUID& other) const
	{
		if (std::get<0>(SUuuid) != std::get<0>(other.SUuuid))
		{
			return std::get<0>(SUuuid) < std::get<0>(other.SUuuid);
		}
		return std::get<1>(SUuuid) < std::get<1>(other.SUuuid);
	}
	SUUUID operator()()
	{
		return SUuuid;
	}
	SUUUID operator+()
	{
		return SUuuid;
	}
	bool empty() const
	{
		return std::get<1>(SUuuid).empty();
	}
	bool operator!() const
	{
		return empty();
	}
	bool operator!()
	{
		return empty();
	}
	explicit operator bool() const
	{
		return validator ? (validator() && !empty()) : !empty();
	}
	explicit operator bool()
	{
		return validator ? (validator() && !empty()) : !empty();
	}
	void clear()
	{
		SceneUnitId& unit = std::get<0>(SUuuid);
		JUUID& uuid = std::get<1>(SUuuid);
		unit = 0;
		uuid = "";
		validator = nullptr;
	}
	SceneUnitId unit() const
	{
		const SceneUnitId& id = std::get<0>(SUuuid);
		return id;
	}
	SceneUnitId unit()
	{
		SceneUnitId& id = std::get<0>(SUuuid);
		return id;
	}
	JUUID uuid() const
	{
		return std::get<1>(SUuuid);
	}
	JUUID uuid()
	{
		return std::get<1>(SUuuid);
	}
};

/*
#define DEF_SCENEOBJECT_ID(TYPE)\
using TYPE##ID = TSUUUID<TYPE, Get##TYPE##SceneObject>;

#define DEF_SCENEOBJECT_ID_DEP(TYPE)\
struct TYPE;\
extern std::unique_ptr<TYPE>& Get##TYPE##SceneObject(SceneUnitId, JUUID uuid);\
DEF_SCENEOBJECT_ID(TYPE)

#define DEF_SCENEOBJECT_ID_HASH(TYPE)\
template <>\
struct std::hash<TYPE##ID>\
{\
	std::size_t operator()(const TYPE##ID& other) const\
	{\
		size_t hash = 0;\
		nostd::hash_combine(hash, std::get<0>(other.SUuuid), std::get<1>(other.SUuuid));\
		return hash;\
	}\
}
*/

// Create a new UUID (v4) cross-platform (Web / Desktop)
inline std::string getUUID()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint32_t> dis(0, 15);
    static std::uniform_int_distribution<uint32_t> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;
    
    for (int i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4"; // UUID Versión 4
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen); // Bits de variante (8, 9, a o b)
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 12; i++) {
        ss << dis(gen);
    }

    return ss.str();
}

inline int FindSelectableIndex(auto selectables, nlohmann::json& json, auto att)
{
	auto& value = json.at(att);
	return static_cast<int>(std::find_if(selectables.begin(), selectables.end(), [value](JUUIDName uuidName)
		{
			return value == std::get<0>(uuidName);
		}
	) - selectables.begin());
}

inline std::function<std::vector<JUUIDName>()> SortUUIDNameByName(std::function<std::vector<JUUIDName>()> getUUIDNames)
{
	return [getUUIDNames]()
		{
			std::vector<JUUIDName> uuidNames = getUUIDNames();
			std::sort(uuidNames.begin(), uuidNames.end(), [](JUUIDName a, JUUIDName b)
				{
					return std::get<1>(a) < std::get<1>(b);
				}
			);
			return uuidNames;
		};
};

inline std::function<std::vector<JUUIDName>()> SortUUIDSUNameByName(SceneUnitId id, std::function<std::vector<JUUIDName>(SceneUnitId)> getUUIDNames)
{
	return [getUUIDNames, id]()
		{
			std::vector<JUUIDName> uuidNames = getUUIDNames(id);
			std::sort(uuidNames.begin(), uuidNames.end(), [](JUUIDName a, JUUIDName b)
				{
					return std::get<1>(a) < std::get<1>(b);
				}
			);
			return uuidNames;
		};
};

inline void SortUUIDByName(std::vector<JUUIDName>& uuidNames)
{
	std::sort(uuidNames.begin(), uuidNames.end(), [](JUUIDName a, JUUIDName b)
		{
			return std::get<1>(a) < std::get<1>(b);
		}
	);
}

inline JNAME GetName(JUUID uuid, auto getContainedObjects)
{
	auto& containedObjects = getContainedObjects();
	return containedObjects.contains(uuid) ? std::get<0>(containedObjects.at(uuid)) : "";
}

inline JNAME GetSUName(SceneUnitId id, JUUID uuid, auto getContainedObjects)
{
	auto& containedObjects = getContainedObjects(id);
	return containedObjects.contains(uuid) ? std::get<0>(containedObjects.at(uuid)) : "";
}

inline JUUID GetUUIDByName(JNAME name, auto getContainedObjects)
{
	auto& containedObjects = getContainedObjects();
	for (auto& [uuid, T] : containedObjects)
	{
		if (std::get<0>(T) == name) return uuid;
	}

	return "";
}

inline JUUID GetUUIDBySUName(SceneUnitId id, JNAME name, auto getContainedObjects)
{
	auto& containedObjects = getContainedObjects(id);
	for (auto& [uuid, T] : containedObjects)
	{
		if (std::get<0>(T) == name) return uuid;
	}

	return "";
}

inline std::vector<JNAME> GetNames(auto& items)
{
	std::vector<JNAME> names;
	std::transform(items.begin(), items.end(), std::back_inserter(names), [](auto& pair)
		{
			return std::get<0>(pair.second);
		}
	);
	std::sort(names.begin(), names.end());
	return names;
}

inline std::vector<JUUIDName> GetUUIDsNames(auto& items)
{
	std::vector<JUUIDName> uuidsNames;
	std::transform(items.begin(), items.end(), std::back_inserter(uuidsNames), [](auto& pair)
		{
			return std::make_tuple(pair.first, std::get<0>(pair.second));
		}
	);
	SortUUIDByName(uuidsNames);
	return uuidsNames;
}

inline std::vector<JUUIDName> GetNonHiddenUUIDsNames(auto& items)
{
	std::vector<JUUIDName> uuidsNames;
	for (auto& pair : items)
	{
		auto& obj = std::get<1>(pair.second);
		if (obj->contains("hidden") && obj->at("hidden") == true)
			continue;

		uuidsNames.push_back(std::make_tuple(pair.first, std::get<0>(pair.second)));
	}
	SortUUIDByName(uuidsNames);
	return uuidsNames;
}

#define DEF_TYPE_ID(NAMESPACE, TEMPLATE, TYPE, GETTER) \
namespace NAMESPACE {\
	struct TYPE;\
	extern std::unique_ptr<TYPE>& GETTER(JUUID uuid);\
	using TYPE##ID = TEMPLATE<TYPE, GETTER>;\
}\
namespace std {\
    template <>\
    struct hash<NAMESPACE::TYPE##ID>\
    {\
        std::size_t operator()(const NAMESPACE::TYPE##ID& other) const\
        {\
            return std::hash<std::string>{}(other.uuid);\
        }\
    };\
}

#define DEF_TYPE_CID(NAMESPACE, TEMPLATE, TYPE, GETTER) \
namespace NAMESPACE {\
	struct TYPE;\
	extern TYPE* GETTER(JUUID uuid);\
	using TYPE##ID = TEMPLATE<TYPE, GETTER>;\
}\
namespace std {\
    template <>\
    struct hash<NAMESPACE::TYPE##ID>\
    {\
        std::size_t operator()(const NAMESPACE::TYPE##ID& other) const\
        {\
            return std::hash<std::string>{}(other.uuid);\
        }\
    };\
}

#define DEF_TYPE_SID(NAMESPACE, TEMPLATE, TYPE, GETTER) \
namespace NAMESPACE {\
	struct TYPE;\
	extern std::unique_ptr<TYPE>& GETTER(SceneUnitId id, JUUID uuid);\
	using TYPE##ID = TEMPLATE<TYPE, GETTER>;\
}\
namespace std {\
    template <>\
    struct hash<NAMESPACE::TYPE##ID>\
    {\
        std::size_t operator()(const NAMESPACE::TYPE##ID& other) const\
        {\
			size_t hash = 0;\
			nostd::hash_combine(hash, std::get<0>(other.SUuuid), std::get<1>(other.SUuuid));\
			return hash;\
        }\
    };\
}