#pragma once
/// ===Include=== ///
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"

///=====================================================/// 
/// 近接戦闘Enemy
///=====================================================///
class CloseRangeEnemy : public BaseEnemy {
public:
	CloseRangeEnemy() = default;
	~CloseRangeEnemy();

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;
	// ImGui
	void UpdateImGui()override;
	// 攻撃処理
	void Attack()override;

public: /// ===衝突判定=== ///
	void OnCollision(Collider* collider) override;

public: /// ===Setter=== ///
	// Translate
	void SetTranslate(const Vector3& pos);

private: /// ===変数=== ///

	/// ===突進情報=== ///
	struct ChargeInfo {
		float moveSpeed;
		float stopTime;
	};
	ChargeInfo chargeInfo_;
};
