#pragma once

#include "Primitive.h"
#include <VertexFormats.h>

namespace Primitives
{
#if defined(_EDITOR)

#include <Attributes/JOrder.h>
#include <ConeAtt.h>
#include <JEnd.h>

// #include <Editor/JDrawersDecl.h>
// #include <ConeAtt.h>
// #include <JEnd.h>

#endif

	struct Cone : Primitive
	{
#include <Attributes/JFlags.h>
#include <ConeAtt.h>
#include <JEnd.h>

#include <Attributes/JStr2Flag.h>
#include <ConeAtt.h>
#include <JEnd.h>

#include <Attributes/JDecl.h>
#include <ConeAtt.h>
#include <JEnd.h>

		DEF_STRING2FLAGS_FUNC(Cone, Primitive);

		static constexpr VertexClass VertexClass = VertexClass::POS_NORMAL;
		typedef Vertex<VertexClass> VertexType;

		std::vector<VertexType> vertices;
		std::vector<uint32_t> indices;

		Cone(nlohmann::json& json);
#if defined(_EDITOR)
		void WriteJson(nlohmann::json& j) override;
#endif
		void PrepareMesh() override;

		std::vector<uint32_t> GetIndices();
		std::vector<VertexType> GetVertices();
	};
};
