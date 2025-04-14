#include "ParticleManager.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ParticleManager::~ParticleManager() {
	particles_.clear();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleManager::Initialize() {
	particles_.clear();
}

///-------------------------------------------/// 
/// Partlceの追加
///-------------------------------------------///
void ParticleManager::AddParticle(const std::string & name, std::unique_ptr<ParticleGroup> particle) {
	// マップに追加
	particles_[name] = std::move(particle);
}

///-------------------------------------------/// 
/// 個別のPartlcelの設定
///-------------------------------------------///
void ParticleManager::SetPartlce(const std::string& name, const Vector3& tralslate) {
	particles_[name]->Initialze();
	particles_[name]->SetTranslate(tralslate);
}
// Texture
void ParticleManager::SetTexture(const std::string& name, const std::string& textureName) {
	particles_[name]->SetTexture(textureName);
}

///-------------------------------------------/// 
/// 全てのPartlceの更新
///-------------------------------------------///
void ParticleManager::Update() {
	for (auto& [name, particle] : particles_) {
		particle->Update();
	}
}

///-------------------------------------------/// 
/// 個別のPartlceの描画
///-------------------------------------------///
void ParticleManager::Draw(const std::string & name, BlendMode mode) {
	particles_[name]->Draw(mode);
}

///-------------------------------------------/// 
/// 全てのPartlceの描画
///-------------------------------------------///
void ParticleManager::DrawAll(BlendMode mode) {
	for (auto& [name, particle] : particles_) {
		particle->Draw(mode);
	}
}
