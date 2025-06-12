#pragma once
/// ===Include=== ///
#include "application/Game/Collider/SphereCollider.h"

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

private:

	/// ===BaseData=== ///
	struct BaseData {
		Vector3 translate = { 0.0f, 0.0f, 0.0f };
		Quaternion rotate = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
	BaseData base_;

	// アニメーションのループ再生の切り替え
	bool isChagAnimation_ = false;
};

