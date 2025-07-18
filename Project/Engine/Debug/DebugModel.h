#pragma once
/// ===Include=== ///
#include "Engine/Collider/OBBCollider.h"

///=====================================================/// 
/// DebugModel
///=====================================================///
class DebugModel : public OBBCollider {
public:
	DebugModel() = default;
	~DebugModel();

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

};

