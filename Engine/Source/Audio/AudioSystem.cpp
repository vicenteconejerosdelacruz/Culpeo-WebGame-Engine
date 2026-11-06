#include "AudioSystem.h"
// #include <Audio.h>
// #undef _WIN32 // Evita que em++ en Windows active las rutas de ensamblador x86 de miniaudio

// #define MA_NO_CPU_DETECTION
// #define MA_NO_SIMD
// #define MA_NO_DECODING_MP3
// #define MA_NO_ENCODING
// #define MA_NO_GENERATION
// #define MINIAUDIO_IMPLEMENTATION
// #include <miniaudio.h>

namespace AudioSystem {

	// std::unique_ptr<DirectX::AudioEngine> audioEngine;
	// DirectX::AudioListener listener;
	
	ma_engine audioEngine;

	ma_engine* GetAudioEngine()
	{
		return &audioEngine;
	}

	void InitAudio()
	{
		ma_result result = ma_engine_init(NULL, &audioEngine);
		if (result != MA_SUCCESS) {
			// Error en la inicialización
			std::cerr << "Error al inicializar el motor de audio: " << result << std::endl;
			return;
		}
		//initialize DirectXTK Audio Engine
		//inicializar el Audio Engine de DirectXTK
		/*
		AUDIO_ENGINE_FLAGS audioFlags = AudioEngine_EnvironmentalReverb | AudioEngine_ReverbUseFilters;
#ifdef _DEBUG
		audioFlags |= AudioEngine_Debug;
#endif
		audioEngine = std::make_unique<DirectX::AudioEngine>(audioFlags);
		*/
	}

	void ShutdownAudio()
	{
		// audioEngine = nullptr;
	}

	void UpdateAudio()
	{
// #if defined(_DEVELOPMENT)
// 		PIXScopedEvent(0, L"UpdateAudio");
// #endif
// 		GetAudioEngine()->Update();
	}

	// DirectX::AudioListener& GetAudioListener()
	// {
	// 	return listener;
	// }

	void UpdateListener(XMFLOAT3 position, XMFLOAT3 fw, XMFLOAT3 up)
	{
		ma_engine_listener_set_position(&audioEngine, 0, position.x, position.y, position.z);
		ma_engine_listener_set_direction(&audioEngine, 0, fw.x, fw.y, fw.z);
    	ma_engine_listener_set_world_up(&audioEngine, 0, up.x, up.y, up.z);
	}

	// std::unique_ptr<DirectX::AudioEngine>& GetAudioEngine()
	// {
	// 	return audioEngine;
	// }
}
