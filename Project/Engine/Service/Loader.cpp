#include "Loader.h"

#include "Engine/Managers/TextureManager.h"
#include "Engine/Managers/ModelManager.h"
#include "Engine/Managers/AudioManager.h"
#include "Engine/Managers/CSVManager.h"

#include "Engine/Core/Mii.h"

#include <cassert>

/// ===宣言=== ///
TextureManager* Loader::textureManager_ = nullptr;
ModelManager* Loader::modelManager_ = nullptr;
AudioManager* Loader::audioManager_ = nullptr;
CSVManager* Loader::csvManager_ = nullptr;

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Loader::Inititalze(TextureManager* texture, ModelManager* model, AudioManager* audio, CSVManager* csv) {
	assert(texture);
	assert(model);
	assert(audio);
	assert(csv);
	
	// 生成
	textureManager_ = texture;
	modelManager_ = model;
	audioManager_ = audio;
	csvManager_ = csv;
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void Loader::Finalize() {
	textureManager_ = nullptr;
	modelManager_ = nullptr;
	audioManager_ = nullptr;
	csvManager_ = nullptr;
}

///-------------------------------------------/// 
/// テクスチャ
///-------------------------------------------///
void Loader::LoadTexture(const std::string& key, const std::string& filePath) {
	textureManager_->LoadTexture(key, filePath);
}

///-------------------------------------------/// 
/// モデル
///-------------------------------------------///
void Loader::LoadModel(const std::string& filename) {
	modelManager_->LoadModel("Resource", filename); 
}

///-------------------------------------------/// 
/// csv
///-------------------------------------------///
void Loader::LoadCSV(const std::string& filename) {
	csvManager_->Load(filename);
}

///-------------------------------------------/// 
/// WAVE
///-------------------------------------------///
void Loader::LoadWave(const std::string& key, const std::string& filename) {
	audioManager_->Load(key, filename, false); 
}

///-------------------------------------------/// 
/// MP3
///-------------------------------------------///
void Loader::LoadMP3(const std::string & key, const std::string & filename) {
	audioManager_->Load(key, filename, true); 
}

///-------------------------------------------/// 
/// 音声データの解放
///-------------------------------------------///
void Loader::UnloadSound(const std::string& key) {
	audioManager_->Unload(key);
}