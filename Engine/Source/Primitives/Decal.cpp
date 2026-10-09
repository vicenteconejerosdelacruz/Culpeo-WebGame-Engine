#include "Decal.h"

using namespace Primitives;

static std::vector<uint32_t> indices = {
	 2, 1, 0, 1, 2, 3 //+Y
};

static std::vector<Decal::VertexType> vertices = {
	{ FLOAT3(1.0f,  1.0f,  0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(-1.0f,  1.0f,  0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f,  0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, -1.0f,  0.0f), FLOAT2(0.0f, 1.0f) },
};

namespace Primitives
{
	std::vector<uint32_t> Decal::GetIndices()
	{
		return indices;
	}

	std::vector<Decal::VertexType> Decal::GetVertices()
	{
		return vertices;
	}
};
