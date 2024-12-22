#include "Object.h"

///-------------------------------------------/// 
/// 移動
///-------------------------------------------///
void Object::Move(float velocity) {
	position_.x += velocity;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
Vector2 Object::GetPosition() const { return position_; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void Object::SetPosition(const Vector2& pos) { position_ = pos; }
