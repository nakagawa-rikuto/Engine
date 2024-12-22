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

private:

	Vector2 position_;
	Vector2 velocity_;
	std::unique_ptr<Sprite> sprite_;
};