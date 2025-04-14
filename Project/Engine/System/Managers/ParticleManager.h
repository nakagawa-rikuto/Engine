#pragma once
/// ===include=== ///
// c++
#include <map>
#include <memory>
#include <string>
// PartlceGroup
#include "Engine/Graphics/Particle/ParticleGroup.h"

///=====================================================/// 
/// ParticleManager
///=====================================================///
class ParticleManager {

	ParticleManager();
	~ParticleManager();

    // 初期化
    void Initialize();

    // パーティクルの追加
    void AddParticle(const std::string& name, std::unique_ptr<ParticleGroup> particle);

    // パーティクルの設定
    void SetPartlce(const std::string& name, const Vector3& tralslate);
    // テクスチャの設定
    void SetTexture(const std::string& name, const std::string& textureName);

    // パーティクルの全体更新
    void Update();

    // パーティクルの描画
    void Draw(const std::string& name, BlendMode mode = BlendMode::KBlendModeNormal);

    // すべて描画
    void DrawAll(BlendMode mode = BlendMode::KBlendModeNormal);

private:
    std::map<std::string, std::unique_ptr<ParticleGroup>> particles_;
};

