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

    // パーティクルが発生したかのフラグ
    bool hasExploded_; 

    // Ring
	const uint32_t kRingDivide = 32; // リングの分割数
	const float kOuterRadius = 1.0f; // リングの外半径
	const float kInnerRadius = 0.2f; // リングの内半径
	const float radianPreDivide = 2.0f * std::numbers::pi_v<float> / float(kRingDivide); // リングの分割数あたりのラジアン




private:
    ParticleData MakeParticle(std::mt19937& randomEngine, const Vector3& translate)override;
    // エミっと
    std::list<ParticleData> Emit(const Group& group, std::mt19937& randomEngine)override;
};

