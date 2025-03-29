#include "Collider.h"

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void Collider::SetWorldTranslate(const Vector3& translate) { translate_ = translate; }

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Translate
Vector3 Collider::GetWorldTranslate() const { return translate_; }