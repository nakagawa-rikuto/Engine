#include "ParticleEmitter.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleEmitter::Initialze(const std::string& filename) {
	particle_ = std::make_unique<ParticleGroup>();
	particle_->Initialze(filename, MaxInstance_);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void ParticleEmitter::Update() {

}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ParticleEmitter::Draw(BlendMode mode) {

}
