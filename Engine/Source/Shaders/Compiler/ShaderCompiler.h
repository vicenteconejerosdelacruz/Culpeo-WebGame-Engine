#pragma once

// #include "CustomIncludeHandler.h"
// #include <Shader/Shader.h>

// namespace Templates
// {
// 	struct ShaderInstance;
// };
// using namespace Templates;

struct ShaderResourceInfo {
	uint32_t group;
	uint32_t binding;
	std::string variableName; // Ej: "camera", "modelUniform"
	std::string typeName;     // Ej: "Camera", "Model", "texture_depth_2d_array"

	void dump()
	{
		std::cout << variableName << ":" << typeName << " group(" << group << ") binding(" << binding << ")" << std::endl;
	}
};

struct VertexAttributeInfo {
    uint32_t location;
    std::string name;

	void dump()
	{
		std::cout << name << " Location:" << location << std::endl;
	}
};


namespace ShaderCompiler {

	inline static std::unordered_map<JShaderType, std::wstring> shaderEntryPoint =
	{
		{ VERTEX_SHADER, L"main_vs"	},
		{ PIXEL_SHADER, L"main_ps" },
		{ COMPUTE_SHADER, L"main_cs" },
	};

	inline static std::unordered_map<JShaderType, std::wstring> shaderTarget =
	{
		{ VERTEX_SHADER, L"vs_6_5" },
		{ PIXEL_SHADER, L"ps_6_5" },
		{ COMPUTE_SHADER, L"cs_6_5" },
	};

	inline static std::unordered_map<JShaderType, std::wstring> shaderDefine =
	{
		{ VERTEX_SHADER, L"_VERTEX_SHADER" },
		{ PIXEL_SHADER, L"_PIXEL_SHADER" },
		{ COMPUTE_SHADER, L"_COMPUTE_SHADER" },
	};

	// void BuildShaderCompiler();
	std::vector<ShaderResourceInfo> ParseWGSLBindings(const std::string& preprocessedWGSL);
	std::string PreprocessWGSL(
		const std::string& filename, 
		std::unordered_set<std::string>& definedMacros,
		const std::function<std::string(const std::string&)>& loadFileCallback,
		std::unordered_set<std::string>& includedFiles);
	std::vector<VertexAttributeInfo> ParseVertexInputAttributes(const std::string& preprocessedWGSL);
	bool Compile(SceneUnitId id, ShaderInstanceID shaderInstance, Source params, ShaderIncludesDependencies& dependencies);
	// void DestroyShaderCompiler();

	//compiler and utils
	// static ComPtr<IDxcCompiler3> pCompiler;
	// static ComPtr<IDxcUtils> pUtils;
};