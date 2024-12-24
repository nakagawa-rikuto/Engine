#include "Game/2d/Sprite.h"
#include <memory>

///=====================================================/// 
/// エネミー
///=====================================================///
class Enemy {
public:

	Enemy() = default;
	~Enemy();

	void Initialize();
	void Update();
	void Draw();

	Vector2 GetPos()const;
	Vector2 GetSize()const;

private:

	Vector2 postition_ = Vector2(680.0f, 200.0f);
	Vector2 size_ = Vector2(40.0f, 40.0f);
	std::unique_ptr<Sprite> sprite_;
};