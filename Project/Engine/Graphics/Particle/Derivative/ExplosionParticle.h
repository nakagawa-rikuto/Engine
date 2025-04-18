#pragma once
/// ===Include=== ///
#include "Engine/Graphics/Particle/Base/ParticleGroup.h"
// Math
#include "Engine/DataInfo/ColliderData.h"
// c++
#include <random>

///=====================================================/// 
/// 爆破エミッタ
///=====================================================///
class ExplosionParticle : public ParticleGroup {
public:

    ExplosionParticle(); // コンストラクタでの処理は行わない
    ~ExplosionParticle();

    /// ===Override=== ///
    // 初期化
    void Initialze()override;
    // 更新（override）
    void InstancingUpdate(std::list<ParticleData>::iterator it)override;
    // 更新
    void Update() override;
    // 描画
    void Draw(BlendMode mode = BlendMode::KBlendModeNormal) override;
    // クローン
    std::unique_ptr<ParticleGroup> Clone() override;

private:
    // === コピー禁止 === ///
    ExplosionParticle(const ExplosionParticle&) = delete;
    ExplosionParticle& operator=(const ExplosionParticle&) = delete;

    /// ===プライベートフィールド=== ///
    std::mt19937 randomEngine_; // 乱数生成器
    bool hasExploded_; // 爆発が発生したかのフラグ

    // 爆発の設定
    float explosionRadius_; // 爆発の半径
    float maxLifetime_; // パーティクルの最大寿命

    // ランダムパーティクル生成
    ParticleData MakeExplosionParticle(std::mt19937& randomEngine, const Vector3& center);
};