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
    // パーティクルの発生
    void Emit(const std::string& name, const Vector3& translate);
    // テクスチャの設定
    void SetTexture(const std::string& name, const std::string& textureName);

    // パーティクルの全体更新
    void Update();
    // すべて描画
    void DrawAll(BlendMode mode = BlendMode::KBlendModeNormal);

private:
    // 登録されたテンプレート
    std::map<std::string, std::unique_ptr<ParticleGroup>> prototype_;
    // 実際に動いているパーティクル
    std::map<std::string, std::vector<std::unique_ptr<ParticleGroup>>> activeParticles_;
};

