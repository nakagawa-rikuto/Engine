#include "Particle.h"
// ParticleManager
#include "Engine/System/Managers/ParticleManager.h"
// Camera
#include "application/Drawing/3d/Camera.h"
// ServiceLocator
#include "ServiceLocator.h"

///-------------------------------------------/// 
/// 発生
///-------------------------------------------///
void Particle::Emit(const std::string& name, const Vector3& translate) {
	ServiceLocator::GetParticleManager()->Emit(name, translate);
}

///-------------------------------------------/// 
/// テクスチャの設定
///-------------------------------------------///
void Particle::SetTexture(const std::string & name, const std::string & textureName) {
	ServiceLocator::GetParticleManager()->SetTexture(name, textureName);
}

///-------------------------------------------/// 
/// カメラの設定
///-------------------------------------------///
void Particle::SetCamera(const std::string& name, Camera* camera) {
	ServiceLocator::GetParticleManager()->SetCamera(name, camera);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Particle::Update() {
	ServiceLocator::GetParticleManager()->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Particle::Draw(BlendMode mode) {
	ServiceLocator::GetParticleManager()->DrawAll(mode);
}
