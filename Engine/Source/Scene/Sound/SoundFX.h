#pragma once
#include <nlohmann/json.hpp>
#include <Controller.h>
#include <SceneControllerDecl.h>

namespace Scene
{
#if defined(_EDITOR)

#include <Attributes/JOrder.h>
#include "SoundFXAtt.h"
#include <JEnd.h>

// #include <Editor/JDrawersDecl.h>
// #include "SoundFXAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDecl.h>
// #include "SoundFXAtt.h"
// #include <JEnd.h>

// #include <Creator/JRequired.h>
// #include "SoundFXAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDecl.h>
// #include "SoundFXAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDecl.h>
// #include "SoundFXAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDecl.h>
// #include "SoundFXAtt.h"
// #include <JEnd.h>

#endif

	using namespace Game;
	struct SoundFX : Controller
	{
		// inline static const SceneObjectType sceneObjectType = SO_SoundEffects;

#include <Attributes/JFlags.h>
#include "SoundFXAtt.h"
#include <JEnd.h>

#include <Attributes/JStr2Flag.h>
#include "SoundFXAtt.h"
#include <JEnd.h>

#include <Attributes/JDecl.h>
#include "SoundFXAtt.h"
#include <JEnd.h>

		DEF_STRING2FLAGS_FUNC(SoundFX, Controller);

		SoundFX(nlohmann::json& json);
		~SoundFX() {
			// Destroy();
		}

		void Map(SUUUID so) override;
		void Unmap() override;

		// void Initialize() override;
		// static void RegisterScript(Isolate* isolate, Local<ObjectTemplate> tpl, SceneUnitScripting* script);
		// void RegisterScriptInstance(Isolate* isolate, Local<ObjectTemplate> proto, SceneUnitScripting* script) override { SoundFX::RegisterScript(isolate, proto, script); }
		// virtual void SetInitialConditions();
		void BindToScene() override;
		void UnbindFromScene() override;

		void Destroy();

		// void updateRotationQ();
		// XMVECTOR rotationQ();
		XMMATRIX world();
		XMVECTOR fw();

		bool EffectExists();
		// std::unique_ptr<DirectX::SoundEffect>& GetEffect();
		// std::unique_ptr<DirectX::SoundEffectInstance>& GetInstance();
		bool Play();
		bool Stop();
		bool Pause();
		bool Resume();
		// bool IsPlaying() { return GetInstance() != nullptr && GetInstance()->GetState() == DirectX::SoundState::PLAYING; }
		// bool IsPaused() { return GetInstance() == nullptr || GetInstance()->GetState() == DirectX::SoundState::PAUSED; }
		// float Duration() { return (GetEffect()->GetSampleDurationMS() / 1000.0f); }
		void Step(float step);
		float Time() const { return time; }
		bool HasStarted() { return hasStarted; }

		void UpdateEmmiter();

#if defined(_EDITOR)
		RenderableID CreateBillboard(CameraID camera) override;
		// virtual void UpdateBillboard(RenderableID renderable);
		// BoundingBox GetBoundingBox();

		//Gizmo
		// virtual bool CanInteractWithGizmo(ImGuizmo::OPERATION operation);
		void WriteJson(nlohmann::json& j) override;
		// static void GatherFiles(nlohmann::json& json, std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream);
#endif
		// DeleteHook markedForDelete;
		float time = 0.0f;
		bool hasStarted = false;
		SoundInstanceID soundEffectInstance;
		//Transformation
		// XMVECTOR rotationQuaternion;
		// AudioEmitter audioEmitter;
		// SoundInstance soundEffectInstance;
	};

	CONTROLLER_DECL_FULL(SoundFX);

#include <TrackUUID/JDecl.h>
#include "SoundFXAtt.h"
#include <JEnd.h>

// 	void SoundFXsStep(SceneUnitId id, float step);
// 	void DestroySoundEffects();
// 	void DestroySoundEffects(SceneUnitId id);
// 	void DeleteSoundFX(SceneUnitId id, JUUID uuid);
// 	void PlaySounds(SceneUnitId id);
// 	void PauseSounds(SceneUnitId id);
// 	void ResumeSounds(SceneUnitId id);
// 	void StopSounds(SceneUnitId id);
// #if defined(_EDITOR)
// 	void WriteSoundFXsJson(SceneUnitId id, nlohmann::json& json);
// #endif
}
