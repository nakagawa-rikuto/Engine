#pragma once
/// ===Include=== ///
#include "Engine/Core/FileTypeData.h"
// C++
#include <string>

/// ===前方宣言=== ///
class TextureManager;
class ModelManager;
class AudioManager;
class CSVManager;

///=====================================================/// 
/// Load関数
///=====================================================///
class Loader {
public:

	Loader() = default;
	~Loader() = default;

	// 初期化
	static void Inititalze(TextureManager* texture, ModelManager* model, AudioManager* audio, CSVManager* csv);
	// 終了処理
	static void Finalize();
	// テクスチャの読み込み
	static void LoadTexture(const std::string& key, const std::string& filePath);
	// モデルの読み込み
	static void LoadModel(const std::string& filename, ModelFileType type = ModelFileType::OBJ);
	// CSVの読み込み
	static void LoadCSV(const std::string& filename);
	// 音声データの読み込み
	static void LoadWave(const std::string& key, const std::string& filename);
	static void LoadMP3(const std::string& key, const std::string& filename);
	// 音声データの解放
	static void UnloadSound(const std::string& key);

private:
	
	static TextureManager* textureManager_;   // TextureManager
	static ModelManager* modelManager_;       // ModelManager
	static AudioManager* audioManager_;       // AudioMangaer
	static CSVManager* csvManager_;           // CSVManager
	
};

