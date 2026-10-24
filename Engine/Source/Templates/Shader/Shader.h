#pragma once

#include <set>
#include <map>
#include <vector>
#include <memory>
//#include <d3d12shader.h>
//#include <wrl.h>
//#include <wrl/client.h>
//#include <dxcapi.h>
#include <string>
#include <Templates.h>
#include <JTemplate.h>
#include <ShaderCompiler.h>

namespace Templates
{
	inline static const std::string ShadowMapLightsShaderResourceViewName = TextureShaderUsageToString.at(TextureShaderUsage_ShadowMaps);

#if defined(_EDITOR)

#include <Attributes/JOrder.h>
#include "ShaderAtt.h"
#include <JEnd.h>

// #include <Editor/JDrawersDecl.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDecl.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

// #include <Creator/JRequired.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDecl.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDecl.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDecl.h>
// #include "ShaderAtt.h"
// #include <JEnd.h>

#endif

#if defined(_DEVELOPMENT)
	// void ShaderJsonStep();
	// void MonitorShaderChanges(std::string folder);
#endif

	namespace Shader
	{
		inline static const std::string templateName = "shaders.yaml";
		inline static const TemplateType templateType = T_Shaders;
	};

	struct ShaderJson : public JTemplate
	{
		TEMPLATE_DECL(Shader);

#include <Attributes/JFlags.h>
#include "ShaderAtt.h"
#include <JEnd.h>

#include <Attributes/JStr2Flag.h>
#include "ShaderAtt.h"
#include <JEnd.h>

#include <Attributes/JDecl.h>
#include "ShaderAtt.h"
#include <JEnd.h>

		DEF_STRING2FLAGS_FUNC(ShaderJson, JTemplate);

#if defined(_EDITOR)
		void WriteJson(nlohmann::json& j) override;
		// void GatherFiles(std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream) override;
#endif
	};

	struct ShaderInstance;

	TEMPDECL_FULL(Shader);
	TEMPDECL_REFTRACKER(Shader);

	struct ShaderInstance
	{
		JUUID instanceUUID;
		JUUID shaderUUID;
		Source shaderSource;

		//vertex shader semantics(POSITION,TEXCOORD0, etc)
		std::vector<std::string> vsSemantics;

		struct
		{
			ShaderResourceBinding camera = defaultBinding;
			ShaderResourceBinding light = defaultBinding;
			ShaderResourceBinding shadowMaps = defaultBinding;
			ShaderResourceBinding model = defaultBinding;
			ShaderResourceBinding material = defaultBinding;
			ShaderResourceBinding animation = defaultBinding;
		} UniformBufferBinding;

		struct
		{
			ShaderResourceBinding lightsShadowMap = defaultBinding;
			ShaderResourceBinding iblIrradiance = defaultBinding;
			ShaderResourceBinding iblPrefiteredEnv = defaultBinding;
			ShaderResourceBinding iblBRDFLUT = defaultBinding;
		} TextureBinding;

		struct
		{
			ShaderResourceBinding baseTextureSampler = defaultBinding;
		} SamplerBinding;

		//the bytecode(vector of bytes)
		ShaderByteCode byteCode;
		std::vector<ShaderResourceInfo> shaderInfo;
		wgpu::ShaderModule shaderModule;

		ShaderInstance(JUUID uuid) { assert(!!!"do not use"); }
		explicit ShaderInstance(
			SceneUnitId id,
			JUUID instance_uuid,
			JUUID uuid, Source params,
			JUUID objectUUID = ""
		);
		~ShaderInstance() {}
		
		void CreateVSSemantics();
		void CreateResourcesBinding();
		void CreateShaderModule(SceneUnitId id);
		std::unordered_map<std::string, ShaderResourceBinding> GetBindingMap();
	};
};
