#include "LongRangeEnemy.h"
// Camera
#include "application/Game/Camera/Camera.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/ParticleService.h"
#include "Engine/System/Service/CameraService.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"

// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
LongRangeEnemy::~LongRangeEnemy() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void LongRangeEnemy::Initialize() {

	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "player");
	// Object3dの初期設定
	SetTranslate(baseInfo_.translate);
	SetRotate(baseInfo_.rotate);
	SetScale(baseInfo_.scale);
	SetColor(baseInfo_.color);

	// Sphereの設定
	SphereCollider::Initialize();
	name_ = ColliderName::Enemy;
	sphere_.center = baseInfo_.translate;
	sphere_.radius = 2.0f;

	// BaseEnemyの初期化
	BaseEnemy::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void LongRangeEnemy::Update() {

	if (!player_) return;

	/// ===Behavior遷移の実装=== ///
	if (behaviorRequest_) {
		// 振る舞いを変更
		behavior_ = behaviorRequest_.value();
		// 各振る舞いの初期化
		switch (behavior_) {
			// 移動
		case LongRangeEnemy::Behavior::kMove:
			InitMove();
			break;
		case LongRangeEnemy::Behavior::kAttack:
			InitAttack();
			break;
		}

		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 各振る舞いの更新
	switch (behavior_) {
	case LongRangeEnemy::Behavior::kMove:
		Move();
		break;
	case LongRangeEnemy::Behavior::kAttack:
		Attack();
		break;
	}

	// タイマーを進める
	advanceTimer();

	// BaseEnemyの更新
	BaseEnemy::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void LongRangeEnemy::Draw(BlendMode mode) {
	SphereCollider::Draw(mode);
}

///-------------------------------------------/// 
/// 更新（ImGui）
///-------------------------------------------///
void LongRangeEnemy::UpdateImGui() {
#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Translate", &baseInfo_.translate.x, 0.1f);
	ImGui::DragFloat4("Rotate", &baseInfo_.rotate.x, 0.1f);

	ImGui::DragFloat("timer", &attackInfo_.timer, 0.1f);
	ImGui::End();
#endif // _DEBUG
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void LongRangeEnemy::OnCollision(Collider* collider) {
	// Playerとの当たり判定
	if (collider->GetColliderName() == ColliderName::Player) {
		// Playerの突進に対しての衝突処理
		if (player_->GetCargeFlag()) {
			// パーティクルを発生
			// 現状課題ようで設定
			ParticleService::Emit("Explosion", baseInfo_.translate);
			//ServiceParticle::SetTexture("Cylinder", "gradationLine");
		}
	}
}

///-------------------------------------------/// 
/// 移動処理
///-------------------------------------------///
void LongRangeEnemy::InitMove() {
	moveInfo_.rangeCenter = { attackInfo_.playerPos }; // 移動範囲の中心
	baseInfo_.velocity = { 0.0f, 0.0f, 0.0f }; // 初期速度をリセット
	moveInfo_.timer = 0.5f;
}
void LongRangeEnemy::Move() {

	// 移動範囲の中心との方向ベクトルを計算（XZ平面）
	Vector3 toCenter = moveInfo_.rangeCenter - baseInfo_.translate;

	// 中心からの距離を取得
	float distanceFromCenter = Length(toCenter);

	/// ===移動処理=== ///
	if (moveInfo_.isWating) { /// ===範囲外に出ていた場合=== ///

		baseInfo_.velocity = { 0.0f, 0.0f, 0.0f }; // 待機中は移動しない

		// 向く方向に回転
		UpdateRotationTowards(moveInfo_.direction, 0.2f);

		if (moveInfo_.timer <= 0.0f) {
			// ランダムな時間を設定
			std::uniform_real_distribution<float> timeDist(1.0f, moveInfo_.interval);
			moveInfo_.timer = timeDist(randomEngine_);

			// 移動ベクトルを設定
			baseInfo_.velocity = moveInfo_.direction * moveInfo_.speed;
			moveInfo_.isWating = false; // 待機フラグを解除
		}

	} else if (distanceFromCenter > moveInfo_.range) { /// ===待機中=== ///

		// 方向の設定と待機処理の準備
		PreparNextMove(toCenter);

	} else if (moveInfo_.timer <= 0.0f && !moveInfo_.isWating) { /// ===範囲内かつタイマーが切れていた場合=== ///

		// ランダムな角度と距離を生成
		std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * Math::Pi());
		std::uniform_real_distribution<float> distanceDist(0.0f, moveInfo_.range);
		// ランダムな値の設定
		float angle = angleDist(randomEngine_);
		float distance = distanceDist(randomEngine_);

		// 方向ベクトルを円から算出
		Vector3 offset = {
			std::cos(angle) * distance,
			0.0f,
			std::sin(angle) * distance
		};

		// 移動先の座標を計算
		Vector3 target = moveInfo_.rangeCenter + offset;
		target.y = baseInfo_.translate.y;

		// 方向の設定と待機処理の準備
		PreparNextMove(target - baseInfo_.translate);
	}

	/// ===Behavirの変更=== ///
	if (CheckAttackable() && attackInfo_.timer <= 0.0f && !attackInfo_.isAttack) {
		behaviorRequest_ = Behavior::kAttack;
	}
}
void LongRangeEnemy::PreparNextMove(const Vector3& vector) {
	Vector3 dir = Normalize(vector);
	dir.y = 0.0f; // Y成分を0にしてXZ平面での方向ベクトルを作成
	moveInfo_.direction = Normalize(dir); // 方向を保存

	// 待機時間を設定
	moveInfo_.timer = moveInfo_.waitTime; // 待機時間を設定
	// 待機フラグをtrueに設定
	moveInfo_.isWating = true;
}

///-------------------------------------------/// 
/// 攻撃処理
///-------------------------------------------///
void LongRangeEnemy::InitAttack() {
	// 攻撃情報の初期化
	attackInfo_.isAttack = true;
	baseInfo_.color = { 0.0f, 1.0f, 0.0f, 1.0f }; // 攻撃中は赤色に変更
	// プレイヤー位置を取得
	attackInfo_.playerPos = player_->GetTranslate();

	// プレイヤー位置への方向ベクトル
	Vector3 dir = Normalize(player_->GetTranslate() - baseInfo_.translate);
	attackInfo_.direction = dir; // 方向を保存
}
void LongRangeEnemy::Attack() {

	// 攻撃時はやや速めに回転
	UpdateRotationTowards(attackInfo_.direction, 0.3f);

	// 攻撃終了判定
	if (attackInfo_.isAttack) {
		Vector3 toTarget = attackInfo_.playerPos - baseInfo_.translate;
		if (Length(toTarget) < 1.0f) { // 到達判定のしきい値（0.5fは任意）
			attackInfo_.isAttack = false;
			baseInfo_.velocity = { 0.0f, 0.0f, 0.0f };
			baseInfo_.color = { 1.0f, 0.0f, 1.0f, 1.0f }; // 元の色に戻す（任意）
			attackInfo_.timer = attackInfo_.interval; // クールダウン再設定
			behaviorRequest_ = Behavior::kMove;
		}
	}
}

///-------------------------------------------/// 
/// 攻撃可能かチェック
///-------------------------------------------///
bool LongRangeEnemy::CheckAttackable() {

	// 敵の前方向ベクトル（Y軸回転を使用）
	float yaw = baseInfo_.rotate.y;
	Vector3 forward = {
		std::sinf(yaw),
		0.0f,
		std::cosf(yaw)
	};

	Vector3 toPlayer = player_->GetTranslate() - baseInfo_.translate;
	toPlayer.y = 0.0f;
	float distance = Length(toPlayer);

	// 距離判定
	if (distance <= attackInfo_.distance) {
		Vector3 dirToPlayer = Normalize(toPlayer);
		float dot = Dot(forward, dirToPlayer);
		float angleToPlayer = std::acos(dot); // ラジアン

		// 角度が範囲内かチェック
		if (angleToPlayer <= attackInfo_.range) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

///-------------------------------------------/// 
/// 回転更新関数
///-------------------------------------------///
void LongRangeEnemy::UpdateRotationTowards(const Vector3& direction, float slerpT) {
	if (Length(direction) < 0.001f) return;

	// forward方向からターゲットクォータニオンを作成
	Quaternion targetRotation = Math::LookRotation(direction, Vector3(0.0f, 1.0f, 0.0f));

	// SLerp補間（→ 正規化する！）
	Quaternion result = Math::SLerp(baseInfo_.rotate, targetRotation, slerpT);
	baseInfo_.rotate = Normalize(result); // ★ 正規化でスケール崩れ防止
}

///-------------------------------------------/// 
/// 時間を進める
///-------------------------------------------///
void LongRangeEnemy::advanceTimer() {
	// 無敵タイマーを進める
	moveInfo_.timer -= deltaTime_;

	// 攻撃用のタイマーを進める
	if (attackInfo_.timer > 0.0f) {
		attackInfo_.timer -= deltaTime_;
	}
}