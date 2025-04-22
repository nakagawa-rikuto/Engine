#include "ParticleStorage.h"
// ParitcleManager
#include "Engine/System/Managers/ParticleManager.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ParticleStorage::~ParticleStorage() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleStorage::Initialize(ParticleManager * particleManager) {
	// 
	particleManager_ = particleManager;

	// 追加
	AddParticle();
}

///-------------------------------------------/// 
/// 追加
///-------------------------------------------///
void ParticleStorage::AddParticle() {
	particleManager_->AddParticle("Confetti", std::make_unique<ConfettiParticle>());
	particleManager_->AddParticle("Explosion", std::make_unique<ExplosionParticle>());
	particleManager_->AddParticle("Wind", std::make_unique<WindParticle>());
	particleManager_->AddParticle("HitEffect", std::make_unique<HitEffectParticle>());
}
