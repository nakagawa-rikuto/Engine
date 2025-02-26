#include "Audio.h"
#include "Engine/System/Managers/AudioManager.h"
// ServiceLocator
#include "ServiceLocator.h"

///-------------------------------------------/// 
/// Audioの処理
///-------------------------------------------///
// 音声の再生
void Audio::PlayeSound(const std::string& key, bool loop) { 
	ServiceLocator::GetAudioManager()->Play(key, loop);
}
// 音声の停止
void Audio::StopSound(const std::string& key) { 
	ServiceLocator::GetAudioManager()->Stop(key);
}
// 音声のボリュームの調整
void Audio::VolumeSound(const std::string& key, float volume) { 
	ServiceLocator::GetAudioManager()->SetVolume(key, volume);
}
// 音声の再生速度の調整
void Audio::PitchSound(const std::string& key, float pitch) { 
	ServiceLocator::GetAudioManager()->setPitch(key, pitch);
}
