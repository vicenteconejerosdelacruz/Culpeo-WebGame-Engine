#include "Sound.h"
#include <map>
#include <set>
#include <Utils/FileSystem.h>
// #include <Audio.h>
#include <miniaudio.h>
#include <AudioSystem.h>
// #if defined(_EDITOR)
// #include <Builder/ReleaseBuilder.h>
// #endif
using namespace AudioSystem;
// using namespace DirectX;

namespace Templates
{
#if defined(_EDITOR)

// #include <Editor/JDrawersDef.h>
// #include "SoundAtt.h"
// #include <JEnd.h>

// #include <Editor/JPreviewDef.h>
// #include "SoundAtt.h"
// #include <JEnd.h>

// #include <Creator/JJsonDef.h>
// #include "SoundAtt.h"
// #include <JEnd.h>

// #include <Creator/JDrawersDef.h>
// #include "SoundAtt.h"
// #include <JEnd.h>

// #include <Creator/JValidatorDef.h>
// #include "SoundAtt.h"
// #include <JEnd.h>

#endif

	namespace Sound
	{
		//SoundJsonID -> counter
		std::unordered_map<JUUID, unsigned int> uuidInstanceCount;
		//SoundJsonID -> AssetStream
		std::unordered_map<JUUID, std::unique_ptr<AssetStream>> uuidInstanceStream;
		//SoundJsonID -> set<SoundInstanceID>
		std::unordered_map<JUUID, std::set<JUUID>> soundsInstances;
		//SoundInstanceID -> SoundInstance
		std::unordered_map<JUUID, std::unique_ptr<SoundInstance>> instances;
	};

	TEMPDEF_FULL(Sound);

	// SoundInstance GetSoundEffectInstance(JUUID uuid, unsigned int flags, std::string objectUUID )
	// {
	// 	using namespace Sound;
	// 	// if (uuidToSoundEffects.contains(uuid))
	// 	// {
	// 	// 	uuidInstanceCount[uuid]++;
	// 	// }
	// 	// else
	// 	// {
	// 	// 	std::unique_ptr<SoundJson>& json = GetSoundTemplate(uuid);
	// 	// 	std::wstring path = nostd::StringToWString(defaultSoundsFolder + json->path());
	// 	// 	//uuidToSoundEffects[uuid] = std::make_unique<DirectX::SoundEffect>(GetAudioEngine().get(), path.c_str());
	// 	// 	uuidToSoundEffects[uuid] = std::make_unique<void>(nullptr);
	// 	// 	uuidInstanceCount[uuid] = 1;
	// 	// }
	// 	return std::make_tuple(
	// 		uuid,
	// 		nullptr
	// 		//std::move(uuidToSoundEffects[uuid]->CreateInstance(SOUND_EFFECT_INSTANCE_FLAGS(flags)))
	// 	);
	// }

	SoundInstanceID CreateSoundInstance(JUUID uuid, unsigned int f)
	{
		using namespace Sound;

		if(!uuidInstanceCount.contains(uuid))
		{
			uuidInstanceCount.insert_or_assign(uuid, 0);

			SoundJsonID json = uuid;
			std::string path = defaultSoundsFolder + json->path();
			std::unique_ptr<AssetStream> asset = std::make_unique<AssetStream>(path);
			uuidInstanceStream.insert_or_assign(uuid, std::move(asset));
		}

		std::unique_ptr<SoundInstance> soundInstance = std::make_unique<SoundInstance>(uuid, f);
		
		uuidInstanceCount.at(uuid)++;

		JUUID instanceUUID = soundInstance->instance();
		soundsInstances[uuid].insert(instanceUUID);
		instances.insert_or_assign(instanceUUID, std::move(soundInstance));
		return instances.at(instanceUUID)->instance;
	}

	std::unique_ptr<SoundInstance>& GetSoundInstance(JUUID uuid)
	{
		using namespace Sound;
		return instances.at(uuid);
	}

	bool SoundEffectExists(JUUID uuid)
	{
		using namespace Sound;
		//return uuidToSoundEffects.contains(uuid);
		return false;
	}

	// std::unique_ptr<DirectX::SoundEffect>& GetSoundEffect(JUUID uuid)
	// {
	// 	using namespace Sound;
	// 	return uuidToSoundEffects.at(uuid);
	// }

	void DestroySoundEffectInstance(JUUID uuid, SoundInstance& soundEffectInstance)
	{
		// using namespace Sound;
		// uuidInstanceCount[uuid]--;
		// soundEffectInstance = std::make_tuple("", nullptr);
		// if (uuidInstanceCount[uuid] == 0)
		// {
		// 	uuidToSoundEffects.erase(uuid);
		// 	uuidInstanceCount.erase(uuid);
		// }
	}

	SoundJson::SoundJson(nlohmann::json& json) : JTemplate(json)
	{
		#include <Attributes/JInit.h>
		#include "SoundAtt.h"
		#include <JEnd.h>

		#include <Attributes/JUpdate.h>
		#include "SoundAtt.h"
		#include <JEnd.h>

		// #include <Attributes/JV8Att.h>
		// #include "SoundAtt.h"
		// #include <JEnd.h>
	}

#if defined(_EDITOR)
	void SoundJson::WriteJson(nlohmann::json& j)
	{
		#include <Editor/JWriteJson.h>
		#include "SoundAtt.h"
		#include <JEnd.h>
	}
// 	void SoundJson::GatherFiles(std::set<std::filesystem::path>& filesToCopy, std::set<JUUID>& templates, ThreadSafeStream& logStream)
// 	{
// 		nlohmann::json json = this->json();
// #include <Editor/JReleaseBuilder.h>
// #include "SoundAtt.h"
// #include <JEnd.h>
// 	}
#endif

	SoundInstance::SoundInstance(JUUID uuid, unsigned int f)
	{
		using namespace Sound;

		json = uuid;
		instance = getUUID();
		flags = f;

		auto& asset = uuidInstanceStream.at(uuid);
		std::string contents = asset->get().str();

		decoder = std::make_unique<ma_decoder>(); // O gestionarlo con unique_ptr
    	ma_decoder_config decoderConfig = ma_decoder_config_init(ma_format_unknown, 0, 0);

    	ma_result result = ma_decoder_init_memory(contents.data(), contents.size(), &decoderConfig, decoder.get());
		if (result != MA_SUCCESS) {
			return;
		}

		// 4. Inicializar el ma_sound usando la fuente de datos del decodificador
		// El último parámetro (flags) puede incluir MA_SOUND_FLAG_DECODE si prefieres decodificarlo completo en RAM
		result = ma_sound_init_from_data_source(GetAudioEngine(), decoder.get(), MA_SOUND_FLAG_DECODE, NULL, &sound);
		
		if (result != MA_SUCCESS) {
			ma_decoder_uninit(decoder.get());
			return;
		}

		ma_sound_set_spatialization_enabled(&sound, (flags & Sound::SoundInstanceFlag::Use3D) ? MA_TRUE : MA_FALSE);
	}

	void SoundInstance::Play(bool loop)
	{
        ma_sound_start(&sound);
		ma_sound_set_looping(&sound, loop ? MA_TRUE : MA_FALSE);
    }

    // Método para pausar/detener manteniendo la posición
    void SoundInstance::Stop()
	{
        ma_sound_stop(&sound);
    }

    // Cambiar volumen (0.0f a 1.0f)
    void SoundInstance::SetVolume(float volume)
	{
        ma_sound_set_volume(&sound, volume);
    }

	void SoundInstance::SetPosition(XMFLOAT3 p)
	{
		ma_sound_set_position(&sound, p.x, p.y, p.z);
	}

	/*
	void SoundJsonStep()
	{
		std::set<SoundJsonID> sounds;
		std::transform(Soundtemplates.begin(), Soundtemplates.end(), std::inserter(sounds, sounds.begin()), [](auto& temps)
			{
				return temps.first;
			}
		);

		std::set<SoundJsonID> rebuildSounds;
		std::copy_if(sounds.begin(), sounds.end(), std::inserter(rebuildSounds, rebuildSounds.begin()), [](auto sound)
			{
				return sound->dirty(SoundJson::Update_path);
			}
		);

		if (rebuildSounds.size() > 0ULL)
		{
			
		}
	}
	*/

	/*
	void ReleaseSoundEffectsInstances()
	{
		//for (auto& [uuid, t] : sounds)
		//{
		//	auto& instances = std::get<3>(t);
		//	instances.clear();
		//}
	}
	*/
}
