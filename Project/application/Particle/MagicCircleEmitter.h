#pragma once
#include "Engine/Particle/ParticleEmitter.h"
#include <random>

///=====================================================/// 
/// MagicCirecleParticle
///=====================================================///
class MagicCircleEmitter : public ParticleEmitter {
public:
	MagicCircleEmitter() = default;
	~MagicCircleEmitter();

    // 初期化
    void Initialze(const std::string& filename = "Particle") override;
    // 更新
    void Update() override;
    // 描画
    void Draw(BlendMode mode = BlendMode::KBlendModeNormal) override;

    // Setter
    void SetPosition(const Vector3& posititon);
    // Imgui
    void UpdateImGui();

private:

    std::mt19937 randomEngine_;
    float verticalSpeed_;    // 上昇速度
    float spiralRadius_;     // 円の半径
    float maxLifetime_;      // パーティクルの最大寿命
    float emitInterval_;     // パーティクル生成間隔
    float rotationSpeed_;    // 回転速度
    float elapsedTime_;      // パーティクル生成の経過時間
    float currentAngle_;     // 現在の角度（円周上）
    float spiralRadiusMax_;  // 螺旋の最大半径
    float spiralRadiusMin_;  // 螺旋の最小半径
    float spiralRadiusSpeed_;  // 螺旋の半径変化速度

    bool isExpanding_;  // 半径が拡大中かどうか

    ParticleData CreateParticle(const Vector3& center, float angle);
};

