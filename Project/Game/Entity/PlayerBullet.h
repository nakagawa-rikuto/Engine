#include "Game/2d/Sprite.h"
#include <memory>

///=====================================================/// 
/// Include
///=====================================================///
class PlayerBullet {
public:
	PlayerBullet() = default;
	~PlayerBullet();

	void Initialze(const Vector2& pos);
	void Update();
	void Draw();

	bool GetFlag() const;
	Vector2 GetPos() const;
	Vector2 GetSize() const;

private:

	float currentTimer_ = 0.0f;
	float LifeTime_ = 3.0f;
	bool isAlive_ = true;
	Vector2 position_ = Vector2(0.0f, 0.0f);
	Vector2 velocity_ = Vector2(0.0f, 4.0f);
	Vector2 size_ = Vector2(30.0f, 30.0f);
	std::unique_ptr<Sprite> sprite_;
};