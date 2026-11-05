#pragma once

struct ShaderResourceInfo {
	uint32_t group;
	uint32_t binding;
	std::string variableName; // Ej: "camera", "modelUniform"
	std::string typeName;     // Ej: "Camera", "Model", "texture_depth_2d_array"
	bool isTexture;
	bool isSampler;

	void dump()
	{
		if(!isTexture && !isSampler)
			std::cout << variableName << ":" << typeName << " group(" << group << ") binding(" << binding << ")" << std::endl;
		else if(isTexture)
			std::cout << variableName << ":" << typeName << " group(" << group << ") binding(" << binding << ") [Texture]" << std::endl;
		else if(isSampler)
			std::cout << variableName << ":" << typeName << " group(" << group << ") binding(" << binding << ") [Sampler]" << std::endl;
		else
			std::cout << variableName << ":" << typeName << " group(" << group << ") binding(" << binding << ") [Unknown]" << std::endl;
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

	inline static std::unordered_map<JShaderType, std::string> shaderEntryPoint =
	{
		{ VERTEX_SHADER, "main_vs"	},
		{ PIXEL_SHADER, "main_ps" },
		{ COMPUTE_SHADER, "main_cs" },
	};

	inline static std::unordered_map<JShaderType, std::string> shaderTarget =
	{
		{ VERTEX_SHADER, "vs_6_5" },
		{ PIXEL_SHADER, "ps_6_5" },
		{ COMPUTE_SHADER, "cs_6_5" },
	};

	inline static std::unordered_map<JShaderType, std::wstring> shaderDefine =
	{
		{ VERTEX_SHADER, L"_VERTEX_SHADER" },
		{ PIXEL_SHADER, L"_PIXEL_SHADER" },
		{ COMPUTE_SHADER, L"_COMPUTE_SHADER" },
	};

	std::vector<ShaderResourceInfo> ParseWGSLBindings(const std::string& preprocessedWGSL);
	std::string PreprocessWGSL(
		const std::string& filename, 
		std::unordered_set<std::string>& definedMacros,
		const std::function<std::string(const std::string&)>& loadFileCallback,
		std::unordered_set<std::string>& includedFiles);
	std::vector<VertexAttributeInfo> ParseVertexInputAttributes(const std::string& preprocessedWGSL);
	bool Compile(SceneUnitId id, ShaderInstanceID shaderInstance, Source params, ShaderIncludesDependencies& dependencies);
};