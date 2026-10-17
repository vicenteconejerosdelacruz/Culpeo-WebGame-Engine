#include <utility>

#if defined(JEXPOSE_V8_ATT)

#define JCLASS(CLASS,GETJOBJECTS) 
#define JTYPE(TYPE,VALUE)
#define JEXPOSE(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<TYPE>::Read, &V8Converter<TYPE>::Update, Update_##ATT };
#define JEXPOSE_CALLBACK(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<TYPE>::Read, &V8Converter<TYPE>::Update, Update_##ATT };
#define JEXPOSE_TRANSFORM(TYPE,ATT,TOTYPE,FROMTYPE,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<TYPE>::Read, &V8Converter<TYPE>::Update, Update_##ATT };
#define JEXPOSE_TRANSFORM_CALLBACK(TYPE,ATT,TOTYPE,FROMTYPE,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<TYPE>::Read, &V8Converter<TYPE>::Update, Update_##ATT };
#define JEXPOSE_ENUM(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { V8ConverterEnum<TYPE>::Read(StringTo##TYPE), V8ConverterEnum<TYPE>::Update(StringTo##TYPE), Update_##ATT };
#define JEXPOSE_VECTOR(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<std::vector<TYPE>>::Read, &V8Converter<std::vector<TYPE>>::Update, Update_##ATT };
#define JEXPOSE_VECTOR_TRANSFORM(TYPE,ATT,TOTYPE,FROMTYPE,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<std::vector<TYPE>>::Read, &V8Converter<std::vector<TYPE>>::Update, Update_##ATT };
#define JEXPOSE_SET(TYPE,ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<std::set<TYPE>>::Read, &V8Converter<std::set<TYPE>>::Update, Update_##ATT };
#define JEXPOSE_MAP_TRANSFORM(KEYTYPE,VALUETYPE,ATT,TOTYPE,FROMTYPE,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<KEYTYPE>::Read, &V8Converter<KEYTYPE>::Update, Update_##ATT };
#define JEXPOSE_MAP_OBJECT(TYPE, ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<TYPE>::Read, &V8Converter<TYPE>::Update, Update_##ATT };
#define JEXPOSE_VECTOR_OBJECT(TYPE, ATT,INITIAL,JEDVALUETYPE,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<std::vector<TYPE*>>::Read, &V8Converter<std::vector<TYPE*>>::Update, Update_##ATT };
#define JEXPOSE_FLAG(ATT, INITIAL, GETFLAGSVALUESFUNCTION,UPDATEMASK,REQUIREDTOCREATE) propertyRegistry[#ATT] = { &V8Converter<unsigned int>::Read, &V8Converter<unsigned int>::Update, Update_##ATT };
#define JPREVIEW(NAME,JEDVALUETYPE)
#define JTRACKUUID(CLASS,NAME,LIMIT,COND)

#endif