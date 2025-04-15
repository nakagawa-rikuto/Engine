#include "ParticleManager.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ParticleManager::~ParticleManager() {
	prototype_.clear();
	activeParticles_.clear();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleManager::Initialize() {
	prototype_.clear();
	activeParticles_.clear();
}

///-------------------------------------------/// 
/// Partlceの追加
///-------------------------------------------///
void ParticleManager::AddParticle(const std::string & name, std::unique_ptr<ParticleGroup> particle) {
	if (prototype_.find(name) == prototype_.end()) {
		prototype_[name] = std::move(particle);
	}
}

///-------------------------------------------/// 
/// 発生
///-------------------------------------------///
void ParticleManager::Emit(const std::string& name, const Vector3& translate) {
	auto it = prototype_.find(name);
	if (it == prototype_.end()) return;

	std::unique_ptr<ParticleGroup> newParticle = it->second->Clone();
	newParticle->Initialze();
	newParticle->SetTranslate(translate);
	activeParticles_[name].push_back(std::move(newParticle));
}
// Texture
void ParticleManager::SetTexture(const std::string& name, const std::string& textureName) {
	auto it = activeParticles_.find(name);
	if (it == activeParticles_.end()) return;

	for (auto& particle : it->second) {
		if (particle) {
			particle->SetTexture(textureName);
		}
	}
}

///-------------------------------------------/// 
/// 全てのPartlceの更新
///-------------------------------------------///
void ParticleManager::Update() {
	for (auto& [name, list] : activeParticles_) {
		for (auto it = list.begin(); it != list.end();) {
			(*it)->Update();
			if ((*it)->IsFinish()) {
				it = list.erase(it);
			} else {
				++it;
			}
		}
	}
}

///-------------------------------------------/// 
/// 全てのPartlceの描画
///-------------------------------------------///
void ParticleManager::DrawAll(BlendMode mode) {
	for (auto& [name, list] : activeParticles_) {
		for (const auto& particle : list) {
			particle->Draw(mode);
		}
	}
}
