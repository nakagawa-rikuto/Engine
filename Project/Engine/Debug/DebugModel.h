#pragma once
/// ===Include=== ///
#include "application/Game/Entity/GameCharacter/GameCharacter.h"

///=====================================================/// 
/// DebugModel
///=====================================================///
class DebugModel : public GameCharacter<OBBCollider> {
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
};

