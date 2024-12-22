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
	Vector2 GetPosition();
	void SetPosition(const Vector2& pos);

private:

	Vector2 postiton_;
	std::unique_ptr<Sprite> sprite_;

	std::list<std::shared_ptr<PlayerBullet>> bullet_;
};