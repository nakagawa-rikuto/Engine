#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Graphics/Particle/Base/ParticleGroup.h"
// Math
#include "Engine/DataInfo/ColliderData.h"
// c++
#include <random>

///=====================================================/// 
/// コンフェッティパーティクル
///=====================================================///
class ConfettiParticle : public ParticleGroup {
public:

    ConfettiParticle(); // コンストラクタでの処理は行わない
    ~ConfettiParticle();

    /// ===Override=== ///
    // 初期化
    void Initialze() override;
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
    ConfettiParticle(const ConfettiParticle&) = delete;
    ConfettiParticle& operator=(const ConfettiParticle&) = delete;

    /// ===プライベートフィールド=== ///
    std::mt19937 randomEngine_; // 乱数生成器
    bool hasExploded_; // パーティクルが発生したかのフラグ

    // コンフェッティ設定
    Vector3 explosionCenter_; // 爆発の中心位置
    float explosionRadius_; // 爆発の半径
    float gravity_; // 重力加速度
    float upwardForce_; // 上方向の初期加速度
    float maxLifetime_; // パーティクルの最大寿命

    // ランダムパーティクル生成
    ParticleData MakeConfettiParticle(std::mt19937& randomEngine, const Vector3& center);
};