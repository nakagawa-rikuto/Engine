#include "DebugModel.h"

#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
DebugModel::~DebugModel() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void DebugModel::Initialize() {

	/// ===モデルを生成=== ///
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "MonsterBall", LightType::PointLight);

	// 設定
	SetTranslate(base_.translate);
	SetRotate(base_.rotate);
	SetScale(base_.scale);
	SetColor(base_.color);

	// コライダーの名前を登録
	name_ = ColliderName::Enemy;
	// OBB情報を登録
	OBBCollider::Initialize();
	obb_.halfSize = { 1.0f, 1.0f, 1.0f };

	// 一回更新を行う
	object3d_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void DebugModel::Update() {

	SetTranslate(base_.translate);
	SetRotate(base_.rotate);
	SetScale(base_.scale);
	SetColor(base_.color);

	object3d_->SetEnviromentMapData(true, 1.0f );

	OBBCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void DebugModel::Draw(BlendMode mode) {
	OBBCollider::Draw(mode);
}

///-------------------------------------------/// 
/// ImGuiの情報表示
///-------------------------------------------///
void DebugModel::ImGuiInfo() {
#ifdef USE_IMGUI
	ImGui::Begin("DebugModel");
	ImGui::DragFloat3("Transform", &base_.translate.x, 0.1f);
	ImGui::DragFloat4("Rotate", &base_.rotate.x, 0.01f);
	ImGui::ColorEdit4("Color", &base_.color.x);
	OBBCollider::Info();
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
