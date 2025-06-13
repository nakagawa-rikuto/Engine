#include "ServiceParticle.h"
// Manager
#include "Engine/System/Managers/ParticleManager.h"
// ServiceLocator
#include "ServiceLocator.h"


void ServiceParticle::AddParticle(const std::string& name, std::unique_ptr<ParticleGroup> particle) {  
    ServiceLocator::GetParticleManager()->AddParticle(name, std::move(particle));  
}

void ServiceParticle::Emit(const std::string& name, const Vector3& translate) {
	ServiceLocator::GetParticleManager()->Emit(name, translate);
}

void ServiceParticle::SetTexture(const std::string& name, const std::string& textureName) {
	ServiceLocator::GetParticleManager()->SetTexture(name, textureName);
}

void ServiceParticle::SetCamera(Camera* camera) {
	ServiceLocator::GetParticleManager()->SetCamera(camera);
}