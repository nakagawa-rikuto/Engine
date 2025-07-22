#pragma once
/// ===Include=== ///
#include "Engine/Collider/SphereCollider.h"

///=====================================================/// 
/// DebugAnimationModel
///=====================================================///
class DebugAnimationModel : public SphereCollider {
public:
	DebugAnimationModel() = default;
	~DebugAnimationModel();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw(BlendMode = BlendMode::KBlendModeNormal) override;

	// ImGui情報を表示
	void ImGuiInfo();

public:
	/// ===衝突応答=== ///
	void OnCollision(Collider* collider) override;

public:

	Vector3& GetTranslate();
	Quaternion& GetRotate();

private:

	/// ===BaseData=== ///
	struct BaseData {
		Vector3 translate = { 0.0f, 0.0f, 0.0f };
		Quaternion rotate = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	};
	BaseData base_;

	struct MoveData {
		float spped = 0.05f;
		Vector3 direction = { 0.0f, 0.0f, 0.0f };
	};
	MoveData info_;

	// アニメーションのループ再生の切り替え
	bool isChagAnimation_ = false;

	const float deltaTime_ = 1.0f / 60.0f;

private:

	// 移動処理
	void Move();
};

