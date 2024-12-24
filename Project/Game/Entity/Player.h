#pragma once
#include "Game/2d/Sprite.h"
#include <memory>
#include <list>

class PlayerBullet;

///=====================================================/// 
/// Player
///=====================================================///
class Player {
public:

	Player() = default;
	~Player();

	void Initialzie();
	void Update();
	void Draw();

public: /// ===Setter, Getter=== ///
	Vector2 GetPosition()const;
	Vector2 GetSize()const;
	Vector2 GetBulletPos() const;
	Vector2 GetBulletSize() const;
	void SetPosition(const Vector2& pos);

private:

	// sprite
	Vector2 postiton_ = Vector2(640.0f, 500.0f);
	Vector2 size_ = Vector2(50.0f, 50.0f);
	bool isShot_;
	std::unique_ptr<Sprite> sprite_;

	// bullet
	std::shared_ptr<PlayerBullet> bullet_;
};