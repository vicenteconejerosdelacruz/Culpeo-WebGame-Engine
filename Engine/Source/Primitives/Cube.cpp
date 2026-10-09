#include "Cube.h"

using namespace Primitives;

static std::vector<uint32_t> indices =
{
		0 , 1, 2, 3, 2, 1, //+Y
		4 , 5, 6, 7, 6, 5, //-X
		8 , 9,10,11,10, 9, //-Z
		12,13,14,15,14,13, //+X
		16,17,18,19,18,17, //+Z
		20,21,22,23,22,21  //-Y
};

static std::vector<Cube::VertexType> vertices =
{
	//+Y
	{ FLOAT3(-1.0f,  1.0f,  1.0f), FLOAT3(0.0f,  1.0f,  0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f,  1.0f,  1.0f), FLOAT3(0.0f,  1.0f,  0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(-1.0f,  1.0f, -1.0f), FLOAT3(0.0f,  1.0f,  0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(1.0f,  1.0f, -1.0f), FLOAT3(0.0f,  1.0f,  0.0f), FLOAT2(0.0f, 1.0f) },

	//-X
	{ FLOAT3(-1.0f,  1.0f,  1.0f), FLOAT3(-1.0f,  0.0f,  0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(-1.0f,  1.0f, -1.0f), FLOAT3(-1.0f,  0.0f,  0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(-1.0f, -1.0f,  1.0f), FLOAT3(-1.0f,  0.0f,  0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT3(-1.0f,  0.0f,  0.0f), FLOAT2(0.0f, 1.0f) },

	//-Z
	{ FLOAT3(-1.0f,  1.0f, -1.0f), FLOAT3(0.0f,  0.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f,  1.0f, -1.0f), FLOAT3(0.0f,  0.0f, -1.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT3(0.0f,  0.0f, -1.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(1.0f, -1.0f, -1.0f), FLOAT3(0.0f,  0.0f, -1.0f), FLOAT2(0.0f, 1.0f) },

	//+X
	{ FLOAT3(1.0f,  1.0f, -1.0f), FLOAT3(1.0f,  0.0f,  0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f,  1.0f,  1.0f), FLOAT3(1.0f,  0.0f,  0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, -1.0f), FLOAT3(1.0f,  0.0f,  0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(1.0f, -1.0f,  1.0f), FLOAT3(1.0f,  0.0f,  0.0f), FLOAT2(0.0f, 1.0f) },

	//+Z
	{ FLOAT3(1.0f,  1.0f,  1.0f), FLOAT3(0.0f,  0.0f, 1.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(-1.0f,  1.0f,  1.0f), FLOAT3(0.0f,  0.0f, 1.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f,  1.0f), FLOAT3(0.0f,  0.0f, 1.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, -1.0f,  1.0f), FLOAT3(0.0f,  0.0f, 1.0f), FLOAT2(0.0f, 1.0f) },

	//-Y
	{ FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT3(0.0f, -1.0f,  0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, -1.0f), FLOAT3(0.0f, -1.0f,  0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(-1.0f, -1.0f,  1.0f), FLOAT3(0.0f, -1.0f,  0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(1.0f, -1.0f,  1.0f), FLOAT3(0.0f, -1.0f,  0.0f), FLOAT2(0.0f, 1.0f) }
};

namespace Primitives
{
#if defined(_EDITOR)

// #include <Editor/JDrawersDef.h>
// #include <CubeAtt.h>
// #include <JEnd.h>

#endif

	Cube::Cube(nlohmann::json& json) : Primitive(json)
	{
#include <Attributes/JInit.h>
#include <CubeAtt.h>
#include <JEnd.h>

#include <Attributes/JUpdate.h>
#include <CubeAtt.h>
#include <JEnd.h>

// #include <Attributes/JV8Att.h>
// #include <CubeAtt.h>
// #include <JEnd.h>
	}

#if defined(_EDITOR)
	void Cube::WriteJson(nlohmann::json& j)
	{
// #include <Editor/JWriteJson.h>
// #include <CubeAtt.h>
// #include <JEnd.h>
		j.erase("uuid");
	}
#endif

	std::vector<uint32_t> Cube::GetIndices()
	{
		return indices;
	}

	std::vector<Cube::VertexType> Cube::GetVertices()
	{
		return vertices;
	}
}
