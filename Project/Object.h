#pragma once
#include "Math/Vector2.h"

///=====================================================/// 
/// オブジェクト
///=====================================================///
class Object {
public:
	Object() = default;
	~Object() = default;

	void Move(float velocity);
	Vector2 GetPosition() const;
	void SetPosition(const Vector2& pos);

private: 
	Vector2 position_;
};

