#pragma once
/// ===Include=== ///
#include "Engine/Particle/ParticleEmitter.h"
// Math
#include "Math/MathData.h"
// c++
#include <random>

///=====================================================/// 
/// 爆破エミッタ
///=====================================================///
class ExplosionEmitter : public ParticleEmitter {
public:

    ExplosionEmitter() = default; // コンストラクタでの処理は行わない
    ~ExplosionEmitter();

    /// ===Override=== ///
    // 初期化
    void Initialze(const std::string& filename = "Particle") override;
    // 更新
    void Update() override;
    // 描画
    void Draw(BlendMode mode = BlendMode::KBlendModeNormal) override;

    // Setter
    void SetPosition(const Vector3& posititon);

private:
    /// ===プライベートフィールド=== ///
    std::mt19937 randomEngine_; // 乱数生成器
    bool hasExploded_; // 爆発が発生したかのフラグ

    // 爆発の設定
    float explosionRadius_; // 爆発の半径
    float maxLifetime_; // パーティクルの最大寿命

    // ランダムパーティクル生成
    ParticleData MakeExplosionParticle(std::mt19937& randomEngine, const Vector3& center);
};

