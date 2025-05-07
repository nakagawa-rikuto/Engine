#pragma once
/// ===Include=== ///
#include "application/Game/Collider/SphereCollider.h"

///=====================================================/// 
/// Player
///=====================================================///
class Player : public SphereCollider {
public:

	Player() = default;
	~Player();

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;
	// 描画
	void Draw()override;
	// ImGui
	void UpdateImGui();

public: /// ===衝突判定=== ///
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///

	Vector3 GetTranslate()const;
	Vector3 GetRotate()const;

private:

	/// ===振る舞い=== ///
	enum class Behavior {
		kRoot,
		kMove,
		kCharge
	};
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	/// ===基本情報=== ///
	struct BaseInfo {
		Vector3 translate = { 0.0f, 1.0f, 0.0f };
		Vector3 rotate = { 0.0f, 0.0f, 0.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
	};
	BaseInfo baseInfo_;

	/// ===移動情報=== ///
	struct MoveInfo {
		float speed;
	};
	MoveInfo moveInfo_;

	/// ===突進情報=== ///
	struct ChargeInfo {
		float speed;
	};
	ChargeInfo chargeInfo_;

private:
	// Root
	void InitRoot();
	void UpdateRoot();
	// Move
	void InitMove();
	void UpdateMove();
	// Charge
	void InitCharge();
	void UpdateCharge();
};

