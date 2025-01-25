#include "AssetLoader.h"

#include "Engine/Managers/TextureManager.h"
#include "Engine/Managers/ModelManager.h"
#include "Engine/Managers/AudioManager.h"
#include "Engine/Managers/CSVManager.h"

#include "Engine/Core/Mii.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void AssetLoader::Inititalze(TextureManager* texture, ModelManager* model, AudioManager* audio) {
	// 生成
	textureManager_ = texture;
	modelManager_ = model;
	audioManager_ = audio;
}

///-------------------------------------------/// 
/// テクスチャ
///-------------------------------------------///
void AssetLoader::LoadTexture(const std::string& key, const std::string& filePath) {
	textureManager_->LoadTexture(key, filePath);
}

///-------------------------------------------/// 
/// モデル
///-------------------------------------------///
void AssetLoader::LoadModel(const std::string& filename) {
	modelManager_->LoadModel("Resource", filename); 
}

///-------------------------------------------/// 
/// csv
///-------------------------------------------///
void AssetLoader::LoadCSV(const std::string& filename) {
	csvManager_->Load(filename);
}

///-------------------------------------------/// 
/// WAVE
///-------------------------------------------///
void AssetLoader::LoadWave(const std::string& key, const std::string& filename) {
	audioManager_->Load(key, filename, false); 
}

///-------------------------------------------/// 
/// MP3
///-------------------------------------------///
void AssetLoader::LoadMP3(const std::string & key, const std::string & filename) {
	audioManager_->Load(key, filename, true); 
}

///-------------------------------------------/// 
/// 音声データの解放
///-------------------------------------------///
void AssetLoader::UnloadSound(const std::string& key) {
	audioManager_->Unload(key);
}