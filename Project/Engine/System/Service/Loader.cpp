#include "Loader.h"

#include "Engine/System/Managers/TextureManager.h"
#include "Engine/System/Managers/ModelManager.h"
#include "Engine/System/Managers/AudioManager.h"
#include "Engine/System/Managers/CSVManager.h"
#include "Engine/System/Managers/AnimationManager.h"

#include <cassert>

/// ===宣言=== ///
TextureManager* Loader::textureManager_ = nullptr;
ModelManager* Loader::modelManager_ = nullptr;
AudioManager* Loader::audioManager_ = nullptr;
CSVManager* Loader::csvManager_ = nullptr;
AnimationManager* Loader::animationManager_ = nullptr;


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Loader::Inititalze(
	TextureManager* texture, ModelManager* model, AudioManager* audio, CSVManager* csv, AnimationManager* animation) {
	assert(texture);
	assert(model);
	assert(audio);
	assert(csv);
	assert(animation);

	// 生成
	textureManager_ = texture;
	modelManager_ = model;
	audioManager_ = audio;
	csvManager_ = csv;
	animationManager_ = animation;
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void Loader::Finalize() {
	textureManager_ = nullptr;
	modelManager_ = nullptr;
	audioManager_ = nullptr;
	csvManager_ = nullptr;
	animationManager_ = nullptr;
}

///-------------------------------------------/// 
/// テクスチャ
///-------------------------------------------///
void Loader::LoadTexture(const std::string& key, const std::string& filePath) {
	// ベースのディレクトリパス
	const std::string& baseDirectorPath = "./Resource/Textures";
	textureManager_->LoadTexture(key, baseDirectorPath + "/" + filePath);
}

///-------------------------------------------/// 
/// モデル
///-------------------------------------------///
void Loader::LoadModel(const std::string& directorPath, const std::string& filename) {
	// ベースのディレクトリパス
	const std::string& baseDirectorPath = "./Resource/Models";
	modelManager_->Load(baseDirectorPath, directorPath, filename);
}

///-------------------------------------------/// 
/// csv
///-------------------------------------------///
void Loader::LoadCSV(const std::string& filename) {
	csvManager_->Load(filename);
}

///-------------------------------------------/// 
/// Animation
///-------------------------------------------///
void Loader::LoadAnimation(const std::string& directorPath, const std::string& filename) {
	// ベースのディレクトリパス
	const std::string& baseDirectorPath = "./Resource/Animations";
	modelManager_->Load(baseDirectorPath, directorPath, filename);
	animationManager_->Load(baseDirectorPath, directorPath, filename);
}

///-------------------------------------------/// 
/// WAVE
///-------------------------------------------///
void Loader::LoadWave(const std::string& key, const std::string& filename) {
	const std::string& directorPath = "./Resource/BGM";
	audioManager_->Load(key, directorPath + "/" + filename, false);
}

///-------------------------------------------/// 
/// MP3
///-------------------------------------------///
void Loader::LoadMP3(const std::string& key, const std::string& filename) {
	const std::string& directorPath = "./Resource/BGM";
	audioManager_->Load(key, directorPath + "/" + filename, true);
}

///-------------------------------------------/// 
/// 音声データの解放
///-------------------------------------------///
void Loader::UnloadSound(const std::string& key) {
	audioManager_->Unload(key);
}

///-------------------------------------------/// 
/// 音声データの一括開放
///-------------------------------------------///
void Loader::AllUnloadSound() {
	audioManager_->UnloadAll();
}
