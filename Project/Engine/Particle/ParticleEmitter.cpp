#include "ParticleEmitter.h"

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Translate
void ParticleEmitter::SetTranslate(const Vector3& translate) { transform_.translate = translate; }
// Rotate
void ParticleEmitter::SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
// Scale
void ParticleEmitter::SetScale(const Vector3& scale) { transform_.scale = scale; }