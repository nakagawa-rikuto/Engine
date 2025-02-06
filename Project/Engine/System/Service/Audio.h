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

	// 初期化
	static void Initialze(AudioManager* audio);
	// 終了処理
	static void Finalize();

	// サウンドの再生
	static void PlayeSound(const std::string& key, bool loop);
	// サウンドの停止
	static void StopSound(const std::string& key);
	// 音量の設定
	static void VolumeSound(const std::string& key, float volume);
	// 再生速度の設定
	static void PitchSound(const std::string& key, float pitch);

private:
	// AudioManager
	static AudioManager* audioManager_;
};

