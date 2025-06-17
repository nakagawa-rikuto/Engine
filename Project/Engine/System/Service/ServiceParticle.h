#pragma once
/// ===include=== ///
// c++
#include <map>
#include <memory>
#include <string>
// PartlceGroup
#include "Engine/Graphics/Particle/Base/ParticleGroup.h"

///=====================================================/// 
/// ServiceParticle
///=====================================================///
class ServiceParticle {
public:

    // パーティクルの追加
    static void AddParticle(const std::string& name, std::unique_ptr<ParticleGroup> particle);
    // パーティクルの発生
    static void Emit(const std::string& name, const Vector3& translate);
    // テクスチャの設定
    static void SetTexture(const std::string& name, const std::string& textureName);
    // カメラの設定
    static void SetCamera(Camera* camera);
};
