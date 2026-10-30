#define JEXPOSE_EDITOR_WRITE_JSON

#define JCLASS(CLASS,GETJOBJECTS) j = nlohmann::json(get_ref<nlohmann::json::object_t&>());
#define JTYPE(TYPE,VALUE)
#define JEXPOSE(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)
#define JEXPOSE_CALLBACK(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)
#define JEXPOSE_TRANSFORM(TYPE,ATT,TOTYPE,FROMTYPE,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)
#define JEXPOSE_TRANSFORM_CALLBACK(TYPE,ATT,TOTYPE,FROMTYPE,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)
#define JEXPOSE_ENUM(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)
#define JEXPOSE_VECTOR(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)
#define JEXPOSE_VECTOR_TRANSFORM(TYPE,ATT,TOTYPE,FROMTYPE,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)
#define JEXPOSE_SET(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)
#define JEXPOSE_MAP_TRANSFORM(KEYTYPE,VALUETYPE,ATT,TOTYPE,FROMTYPE,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)
#define JEXPOSE_MAP_OBJECT(TYPE, ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)\
	nlohmann::json& objects##ATT = (*this)[#ATT];\
	std::map<std::string, nlohmann::json> jmap##ATT;\
	for (nlohmann::json::iterator it = objects##ATT.begin(); it != objects##ATT.end(); it++)\
	{\
		nlohmann::json jreplacement;\
		auto& obj = Get##TYPE(it.value());\
		obj->WriteJson(jreplacement);\
		jmap##ATT.insert_or_assign(it.key(),jreplacement); \
	}\
	j[#ATT] = nlohmann::json::object({});\
	for(auto &[k,v]:jmap##ATT)\
	{\
		j[#ATT][k]=v;\
	}
#define JEXPOSE_VECTOR_OBJECT(TYPE, ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE)\
	nlohmann::json& objects##ATT = (*this)[#ATT];\
	j[#ATT] = nlohmann::json::array({});\
	for(size_t index = 0ULL; index < objects##ATT.size(); index++)\
	{\
		nlohmann::json jreplacement;\
		auto& obj = Get##TYPE(objects##ATT[index]);\
		obj->WriteJson(jreplacement);\
		j[#ATT].push_back(jreplacement);\
	}
#define JEXPOSE_FLAG(ATT, INITIAL, GETFLAGSVALUESFUNCTION,UPDATEMASK,REQUIREDTOCREATE)
#define JPREVIEW(NAME,JEDVALUETYPE)
#define JTRACKUUID(CLASS,NAME,LIMIT,COND)