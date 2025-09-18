#include "DebugModel.h"
// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
DebugModel::~DebugModel() {
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void DebugModel::Initialize() {

	/// ===モデルを生成=== ///
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "MonsterBall", LightType::PointLight);

	// コライダーの名前を登録
	name_ = ColliderName::Enemy;
	// OBB情報を登録
	GameCharacter::Initialize();
	obb_.halfSize = { 1.0f, 1.0f, 1.0f };

	// 一回更新を行う
	GameCharacter::Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void DebugModel::Update() {

	object3d_->SetEnviromentMapData(true, 1.0f );

	GameCharacter::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void DebugModel::Draw(BlendMode mode) {
	GameCharacter::Draw(mode);
}

///-------------------------------------------/// 
/// ImGuiの情報表示
///-------------------------------------------///
void DebugModel::ImGuiInfo() {
#ifdef USE_IMGUI
	ImGui::Begin("DebugModel");
	GameCharacter::Information();
	ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突時の処理
///-------------------------------------------///
void DebugModel::OnCollision(Collider * collider) {
	if (collider->GetColliderName() == ColliderName::Player) {
		//base_.color = { 0.0f, 0.0f, 1.0f, 1.0f }; // 赤色に変更
	}
}
