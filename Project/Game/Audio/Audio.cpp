#include "Audio.h"
#include "Engine/Managers/AudioManager.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Audio::Initialze(AudioManager* audio) {
	audioManager_ = audio;
}

///-------------------------------------------/// 
/// 
///-------------------------------------------///
// 音声の再生
void Audio::PlayeSound(const std::string& key, bool loop) { audioManager_->Play(key, loop); }
// 音声の停止
void Audio::StopSound(const std::string& key) { audioManager_->Stop(key); }
// 音声のボリュームの調整
void Audio::VolumeSound(const std::string& key, float volume) { audioManager_->SetVolume(key, volume); }
// 音声の再生速度の調整
void Audio::PitchSound(const std::string& key, float pitch) { audioManager_->setPitch(key, pitch); }
