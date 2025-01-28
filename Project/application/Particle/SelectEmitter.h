#pragma once
#include "Engine/Particle/ParticleEmitter.h"
#include <random>

///=====================================================/// 
/// SelectParticle
///=====================================================///
class SelectEmitter : public ParticleEmitter {
	SelectEmitter() = default;
	~SelectEmitter();

    /// ===Override=== ///
   // 初期化
    void Initialze(const std::string& filename = "Particle") override;
    // 更新
    void Update() override;
    // 描画
    void Draw(BlendMode mode = BlendMode::KBlendModeNormal) override;
    // Setter
    void SetPosition(const Vector3& posititon);
    void SetAABB(const Vector3& min, const Vector3& max);
    void SetActive(bool flag);
private:

    std::mt19937 randomEngine_;     // 乱数生成器
    Vector3 aabbMin_;               // AABBの最小点
    Vector3 aabbMax_;               // AABBの最大点
    bool isActive_;                 // 粒子生成を有効にするフラグ

    ParticleData CreateParticle();  // 粒子を生成する関数
};

