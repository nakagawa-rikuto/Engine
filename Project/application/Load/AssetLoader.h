#pragma once
/// ===Include=== ///
// C++
#include <string>
#include <memory>

/// ===前方宣言=== ///
class TextureManager;
class ModelManager;
class AudioManager;

///=====================================================/// 
/// Load関数
///=====================================================///
class AssetLoader {
public:

	AssetLoader() = default;
	~AssetLoader() = default;

	// 初期化
	void Inititalze(TextureManager* texture, ModelManager* model, AudioManager* audio);
	// テクスチャの読み込み
	void LoadTexture(const std::string& filePath);
	// モデルの読み込み
	void LoadModel(const std::string& filename);
	// 音声データの読み込み
	void LoadWave(const std::string& key, const std::string& filename);
	void LoadMP3(const std::string& key, const std::string& filename);
	// 音声データの解放
	void UnloadSound(const std::string& key);

private:
	
	TextureManager* textureManager_;   // TextureManager
	ModelManager* modelManager_;       // ModelManager
	AudioManager* audioManager_;       // AudioMangaer
	
};

