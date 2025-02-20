#include "ParticleEmitter.h"
// Service
#include "Engine/System/Service/Getter.h"
// Math
#include "Math/sMath.h"


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleEmitter::Initialze(const std::string& filename) {
    /// ===パーティクルグループの初期化=== ///
    emitter_.particle = std::make_unique<ParticleGroup>();
    emitter_.particle->Initialze(filename, emitter_.maxInstance);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void ParticleEmitter::InstancingUpdate(std::list<ParticleData>::iterator it) {
    // WVPマトリクス
    Matrix4x4 worldMatrix = MakeAffineMatrix(it->transform.scale, it->transform.rotate, it->transform.translate);
    Matrix4x4 cameraMatrix = Inverse4x4(MakeAffineMatrix(emitter_.cameraTransform.scale, emitter_.cameraTransform.rotate, emitter_.cameraTransform.translate));
    Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(Getter::GetWindowWidth()) / static_cast<float>(Getter::GetWindowHeight()), 0.1f, 100.0f);
    Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(cameraMatrix, projectionMatrix));

    // インスタンシングデータを設定
    emitter_.particle->SetInstancingData(emitter_.numInstance, it->color, wvpMatrix, worldMatrix);
    ++emitter_.numInstance;
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ParticleEmitter::Draw(BlendMode mode) {
	emitter_.particle->Darw(emitter_.numInstance, mode);
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Translate
void ParticleEmitter::SetTranslate(const Vector3& translate) { emitter_.transform.translate = translate; }
// Rotate
void ParticleEmitter::SetRotate(const Vector3& rotate) { emitter_.transform.rotate = rotate; }
// Scale
void ParticleEmitter::SetScale(const Vector3& scale) { emitter_.transform.scale = scale; }
// Texture
void ParticleEmitter::SetTexture(const std::string& fileName) { return emitter_.particle->SetTexture(fileName); }
