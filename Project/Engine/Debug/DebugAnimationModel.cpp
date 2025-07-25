#include "DebugAnimationModel.h"
// Service
#include "Engine/System/Service/InputService.h"
// Math
#include "Math/sMath.h"

#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
DebugAnimationModel::~DebugAnimationModel() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
Vector3& DebugAnimationModel::GetTranslate() {return base_.translate;}
Quaternion& DebugAnimationModel::GetRotate() { return base_.rotate; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void DebugAnimationModel::Initialize() {

	/// ===モデルを生成=== ///
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::AnimationModel, "human", LightType::Lambert);
	SetTranslate(base_.translate);
	SetRotate(base_.rotate);
	SetScale(base_.scale);
	SetColor(base_.color);
	// アニメーションを登録しないとアニメーションが再生されない
	object3d_->SetAnimation("Armature|mixamo.com|Layer0");
	
	// コライダーの名前を登録
	name_ = ColliderName::Player;
	// Sphere情報を登録
	SphereCollider::Initialize();
	sphere_.radius = 0.8f;

	// 一回更新を行う
	object3d_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void DebugAnimationModel::Update() {

	/// ===移動処理=== ///
	Move();

	 /// ===Animationの切り替え=== ///
	if (InputService::TriggerKey(DIK_SPACE)) {
		if (isChagAnimation_) {
			// アニメーションのループを有効化
			object3d_->SetAnimation("Armature|mixamo.com|Layer0", false);
			isChagAnimation_ = false;
		} else {
			// アニメーションのループを無効化
			object3d_->SetAnimation("Armature|mixamo.com|Layer0", true);
			isChagAnimation_ = true;
		}
	}

	// 移動量の加算
	base_.translate += base_.velocity;

	// Objctの更新
	SetTranslate(base_.translate);
	SetRotate(base_.rotate);
	SetScale(base_.scale);
	SetColor(base_.color);

	// 環境マップ
	object3d_->SetEnviromentMapData(true, 1.0f);

	// SphereColliderの更新
	SphereCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void DebugAnimationModel::Draw(BlendMode mode) {
	// SphereColliderの描画
	SphereCollider::Draw(mode);
}

///-------------------------------------------/// 
/// ImGuiの情報表示
///-------------------------------------------///
void DebugAnimationModel::ImGuiInfo() {
#ifdef USE_IMGUI
	ImGui::Begin("DebugAnimationModel");
	ImGui::DragFloat3("Transform", &base_.translate.x, 0.1f);
	ImGui::DragFloat4("Rotate", &base_.rotate.x, 0.01f);
	ImGui::ColorEdit4("Color", &base_.color.x);
	SphereCollider::Info();
	ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突時の処理
///-------------------------------------------///
void DebugAnimationModel::OnCollision(Collider * collider) {

	if (collider->GetColliderName() == ColliderName::Enemy) {
		//base_.color = { 1.0f, 0.0f, 0.0f, 1.0f }; // 赤色に変更
	}
}


///-------------------------------------------/// 
/// 移動処理
///-------------------------------------------///
void DebugAnimationModel::Move() {

	/// ===左スティック入力関数=== ///
	StickState leftStick = InputService::GetLeftStickState(0);

	/// === 徐々に止まる処理 === ///
	// 減速率
	const float deceleration = 0.75f;
	// Velocityが0でないなら徐々に0にする
	if (base_.velocity.x != 0.0f) {
		// 各軸に対して減速適用
		base_.velocity.x *= deceleration;
		// 小さすぎる値は完全に0にスナップ
		if (std::abs(base_.velocity.x) < 0.01f) {
			base_.velocity.x = 0.0f;
		}
	}
	if (base_.velocity.z != 0.0f) {
		base_.velocity.z *= deceleration;
		if (std::abs(base_.velocity.z) < 0.01f) {
			base_.velocity.z = 0.0f;
		}
	}

	/// ===移動処理=== ///
	// 方向の設定
	info_.direction.x = leftStick.x;
	info_.direction.z = leftStick.y;

	// velocityに反映
	base_.velocity = info_.direction * info_.spped;

	/// ===移動方向に沿って回転=== ///
	// 方向が変更されたら
	if (Length(info_.direction) > 0.01f) {
		// 現在のYaw角（Y軸の回転）を取得
		float currentYaw = Math::GetYAngle(base_.rotate);

		// 入力方向から目標のYaw角を取得
		float targetYaw = std::atan2(info_.direction.x, info_.direction.z);

		// 差分を正規化
		float diff = Math::NormalizeAngle(targetYaw - currentYaw);

		// イージング補間（短い方向へ回転）
		float easedYaw = currentYaw + diff * (0.3f);

		// Quaternionに再変換
		base_.rotate = Math::MakeRotateAxisAngle({ 0, 1, 0 }, easedYaw);
	}
}
