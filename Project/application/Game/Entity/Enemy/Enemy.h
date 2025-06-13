#pragma once
/// ===Include=== ///
#include "application/Game/Collider/SphereCollider.h"

/// ===前方宣言=== ///
class Player;

///=====================================================/// 
/// Enemy
///=====================================================///
class Enemy : public SphereCollider {
public:
	Enemy() = default;
	~Enemy();

	// 初期化（GameScene用）
	void Init(Camera* camera, Player* player);

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;
	// ImGui
	void UpdateImGui();

public: /// ===衝突判定=== ///
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///

	Vector3 GetTranslate()const;
	Quaternion GetRotate()const;

private: /// ===変数の宣言=== ///

	Camera* camera_ = nullptr; // カメラ

	Player* player_ = nullptr;

	/// ===基本情報=== ///
	struct BaseInfo {
		Vector3 translate = { 0.0f, 1.0f, 0.0f };
		Quaternion rotate = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };
		Vector4 color = { 1.0f, 0.0f, 1.0f, 1.0f };
		Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	};
	BaseInfo baseInfo_;

private: 
	// 移動処理
	void Move();
};

