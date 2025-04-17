#pragma once
/// ===Include=== ///
// Particle
#include "Engine/Graphics/Particle/Derivative/ConfettiParticle.h"
#include "Engine/Graphics/Particle/Derivative/ExplosionParticle.h"
#include "Engine/Graphics/Particle/Derivative/WindParticle.h"

/// ===前方宣言=== ///
class ParticleManager;

///=====================================================/// 
/// パーティクル保管庫
///=====================================================///
class ParticleStorage {
public:

	ParticleStorage() = default;
	~ParticleStorage();

	// 初期化
	void Initialize(ParticleManager* particleManager);
	
private:
	ParticleManager* particleManager_ = nullptr;

private:
	// パーティクルの追加
	void AddParticle();
};

