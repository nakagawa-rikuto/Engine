#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Graphics/Particle/Base/ParticleGroup.h"
// Math
#include "Engine/DataInfo/ColliderData.h"
// c++
#include <random>

///=====================================================/// 
/// HitEffectParticle
///=====================================================///
class HitEffectParticle : public ParticleGroup {
public:

	HitEffectParticle() = default;
	~HitEffectParticle();

    /// ===Override=== ///
    // 初期化
    void Initialze(const Vector3& translate) override;
    // 更新
    void Update() override;
    // 描画
    void Draw(BlendMode mode = BlendMode::KBlendModeNormal) override;
    // クローン
    std::unique_ptr<ParticleGroup> Clone() override;

private:

    HitEffectParticle(const HitEffectParticle&) = delete;
    HitEffectParticle& operator=(const HitEffectParticle&) = delete;

    bool hasExploded_; // パーティクルが発生したかのフラグ

    ParticleData MakeParticle(std::mt19937& randomEngine, const Vector3& translate)override;
    // エミっと
    std::list<ParticleData> Emit(const Group& group, std::mt19937& randomEngine)override;
};

