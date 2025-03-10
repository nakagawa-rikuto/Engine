#include "ParticleGroup.h"
// Service
#include "Engine/System/Service/Getter.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ParticleGroup::~ParticleGroup() { 
    group_.particles.clear();
    group_.particle.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleGroup::Initialze(const std::string& filename) {
    /// ===パーティクルグループの初期化=== ///
    group_.particle = std::make_unique<ParticleSetUp>();
    group_.particle->Initialze(filename, group_.maxInstance);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void ParticleGroup::InstancingUpdate(std::list<ParticleData>::iterator it) {
    // WVPマトリクス
    Matrix4x4 worldMatrix = Math::MakeAffineEulerMatrix(it->transform.scale, it->transform.rotate, it->transform.translate);
    Matrix4x4 cameraMatrix = Math::Inverse4x4(Math::MakeAffineEulerMatrix(group_.cameraTransform.scale, group_.cameraTransform.rotate, group_.cameraTransform.translate));
    Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(0.45f, static_cast<float>(Getter::GetWindowWidth()) / static_cast<float>(Getter::GetWindowHeight()), 0.1f, 100.0f);
    Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(cameraMatrix, projectionMatrix));

    // インスタンシングデータを設定
    group_.particle->SetInstancingData(group_.numInstance, it->color, wvpMatrix, worldMatrix);
    ++group_.numInstance;
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ParticleGroup::Draw(BlendMode mode) {
	group_.particle->Darw(group_.numInstance, mode);
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Translate
void ParticleGroup::SetTranslate(const Vector3& translate) { group_.transform.translate = translate; }
// Rotate
void ParticleGroup::SetRotate(const Vector3& rotate) { group_.transform.rotate = rotate; }
// Scale
void ParticleGroup::SetScale(const Vector3& scale) { group_.transform.scale = scale; }
// Texture
void ParticleGroup::SetTexture(const std::string& fileName) { return group_.particle->SetTexture(fileName); }
