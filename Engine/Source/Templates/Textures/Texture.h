#pragma once

#include <string>
#include <tuple>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>
#include <Templates.h>
#include <JTemplate.h>
// #if defined(_EDITOR)
// #include <imgui.h>
// #include <DeviceUtils/CommandsProcessor/CommandsProcessor.h>
// using namespace DeviceUtils;
// #endif

enum TextureType;

namespace Templates
{
#if defined(_EDITOR)

#include <Attributes/JOrder.h>
#include "TextureAtt.h"
#include <JEnd.h>

// #include <Editor/JDrawersDecl.h>
// #include "TextureAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDecl.h>
// #include "TextureAtt.h"
// #include <JEnd.h>

// #include <Creator/JRequired.h>
// #include "TextureAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDecl.h>
// #include "TextureAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDecl.h>
// #include "TextureAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDecl.h>
// #include "TextureAtt.h"
// #include <JEnd.h>

// 	void TextureJsonsStep();
// 	void GenerateTexturesDDSFiles();
#endif

	namespace Texture
	{
		inline static const std::string templateName = "textures.yaml";
		inline static const std::string defaultBaseTexture = "Assets/textures/gridmap.dds";
		inline static const std::string defaultNormalMap = "Assets/textures/bumpmapflat.dds";
		inline static const TemplateType templateType = T_Textures;

		inline static const std::string baseTexture = "baseTexture";
		inline static const std::string normalMap = "normalMap";
		inline static const std::string roughnessMap = "roughnessMap";
		inline static const std::string metallicMap = "metallicMap";
		inline static const std::string aoMap = "aoMap";
		inline static const std::string emissiveMap = "emissiveMap";
		inline static const std::string lightsShadowMap = "lightsShadowMap";
		inline static const std::string iblIrradiance = "iblIrradiance";
		inline static const std::string iblPrefiteredEnv = "iblPrefiteredEnv";
		inline static const std::string iblBRDFLUT = "iblBRDFLUT";
	};

	struct TextureJson;
	struct TextureInstance;
// #if defined(_EDITOR)
// 	struct TexturePreview;
// #endif

	struct TextureJson : public JTemplate
	{
		TEMPLATE_DECL(Texture);

#include <Attributes/JFlags.h>
#include "TextureAtt.h"
#include <JEnd.h>

#include <Attributes/JStr2Flag.h>
#include "TextureAtt.h"
#include <JEnd.h>

#include <Attributes/JDecl.h>
#include "TextureAtt.h"
#include <JEnd.h>

		DEF_STRING2FLAGS_FUNC(TextureJson, JTemplate);

#if defined(_EDITOR)
		// virtual void WriteJson(nlohmann::json& j);
		// void GatherFiles(std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream) override;
		// virtual void EditorPreview(size_t flags);
		// virtual void DestroyEditorPreview();
		// void CreatePreviewTexture();

		// std::unique_ptr<TexturePreview> preview;
#endif
	};

	TEMPDECL_FULL(Texture);

#if defined(_DEVELOPMENT)
	// DXGI_FORMAT GetTextureFormat(std::filesystem::path path);
// 	void Create2DDDSFile(TextureJson& json);
// 	void CreateArrayDDSFile(TextureJson& json);
// 	void CreateCubeDDSFile(TextureJson& json);
// 	void CreateCubeDDSFileFromSkyBox(TextureJson& json);
// 	JUUID CreateTextureTemplate(std::string name, DXGI_FORMAT format);
// 	void CreateDDSFile(std::unique_ptr<TextureJson>& tex, bool overwrite = false);
// #if defined(_EDITOR)
// 	void CreateTextureFromJsonDefinition(nlohmann::json& json);
// 	void PreviewTexturesStep(DX::StepTimer& timer);
// #endif
#endif

	struct TextureInstance
	{
#if defined(_EDITOR)
		// TextureInstance(CComPtr<ID3D12GraphicsCommandList2>& commandList, JUUID uuid);
		// TextureInstance(CComPtr<ID3D12GraphicsCommandList2>& commandList, JUUID uuid, unsigned int startFrame);
#endif
		TextureInstance(JUUID uuid) { assert(!!!"do not use"); }
		TextureInstance(SceneUnitId id, JUUID uuid);
		TextureInstance(SceneUnitId id, JUUID uuid, unsigned int startFrame);
		~TextureInstance() {}
		void CreateTextureResource(SceneUnitId id, std::string& path, wgpu::TextureFormat format, TextureType type, unsigned int numFrames, unsigned int nMipMaps, unsigned int startFrame = 0U);
		void ReleaseResources();
		void RebuildResource();

		JUUID materialTexture;
		wgpu::Texture texture;
		wgpu::TextureView textureView;
		std::vector<std::byte> textureData;
	};

	TEMPDECL_REFTRACKER(Texture);

#if defined(_EDITOR)
	// struct TexturePreview
	// {
	// 	bool processorInitialized = false;
	// 	std::unique_ptr<CommandsProcessor> loadingProcessor;
	// 	std::unique_ptr<std::atomic_bool> previewLoaded;
	// 	std::vector<TextureInstanceID> textures;
	// 	int frame;
	// 	bool playing;
	// 	bool looping;
	// 	float time;
	// 	float timeFactor;
	// };
#endif
};

inline auto ToTextureJson(std::vector<JObject*>& json)
{
	std::vector<TextureJsonID> textures;
	std::transform(json.begin(), json.end(), std::back_inserter(textures), [](auto j)
		{
			return std::string(j->at("uuid"));
		}
	);
	return textures;
}
