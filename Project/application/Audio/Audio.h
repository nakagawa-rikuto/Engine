#pragma once
/// ===Include=== ///
#include <string>
#include <memory>

/// ===前方宣言=== ///
class AudioManager;

///=====================================================/// 
/// Audio
///=====================================================///
class Audio {
public:

	Audio() = default;
	~Audio() = default;

	void Initialze(AudioManager* audio);

	// サウンドの再生
	void PlayeSound(const std::string& key, bool loop);
	// サウンドの停止
	void StopSound(const std::string& key);
	// 音量の設定
	void VolumeSound(const std::string& key, float volume);
	// 再生速度の設定
	void PitchSound(const std::string& key, float pitch);

private:
	// AudioManager
	AudioManager* audioManager_;
};

