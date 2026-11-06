#pragma once

#define MA_NO_ENCODING
#define MA_NO_GENERATION
#define MA_NO_DECODE_MP3
#include <miniaudio.h>

namespace AudioSystem {

	ma_engine* GetAudioEngine();
	void InitAudio();
	void ShutdownAudio();
	void UpdateAudio();
	//DirectX::AudioListener& GetAudioListener();
	void UpdateListener(XMFLOAT3 position, XMFLOAT3 fw, XMFLOAT3 up);
	//std::unique_ptr<DirectX::AudioEngine>& GetAudioEngine();
}
