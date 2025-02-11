#pragma once
/// ===Include=== ///
// C++
#include <string>

/// ===前方宣言=== ///
class TextureManager;
class ModelManager;
class AudioManager;
class CSVManager;
class AnimationManager;

///=====================================================/// 
/// Load関数
///=====================================================///
class Loader {
public:

	Loader() = default;
	~Loader() = default;

	// 初期化
	static void Inititalze(TextureManager* texture, ModelManager* model, AudioManager* audio, CSVManager* csv, AnimationManager* animation);
	// 終了処理
	static void Finalize();
	// テクスチャの読み込み
	static void LoadTexture(const std::string& key, const std::string& filePath);
	// モデルの読み込み
	static void LoadModel(const std::string& directorPath, const std::string& filename);
	// CSVの読み込み
	static void LoadCSV(const std::string& filename);
	// Animationの読み込み
	static void LoadAnimation(const std::string& directorPath, const std::string& filename);
	// 音声データの読み込み
	static void LoadWave(const std::string& key, const std::string& filename);
	static void LoadMP3(const std::string& key, const std::string& filename);
	// 音声データの解放
	static void UnloadSound(const std::string& key);
	// 音声データの一括開放
	static void AllUnloadSound();

private:
	
	static TextureManager* textureManager_;     // TextureManager
	static ModelManager* modelManager_;         // ModelManager
	static AudioManager* audioManager_;         // AudioMangaer
	static CSVManager* csvManager_;             // CSVManager
	static AnimationManager* animationManager_; // AnimationManager
};

