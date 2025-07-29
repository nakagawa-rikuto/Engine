#include "CloseRangeEnemy.h"
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
// c++
#include <cstdlib>
#include <ctime>

// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
CloseRangeEnemy::~CloseRangeEnemy() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void CloseRangeEnemy::Initialize() {

	// シードの設定
	srand(static_cast<unsigned int>(time(nullptr)));

	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "player");
	// Object3dの初期設定
	object3d_->SetTranslate(baseInfo_.translate);
	object3d_->SetRotate(baseInfo_.rotate);
	object3d_->SetScale(baseInfo_.scale);
	object3d_->SetColor(baseInfo_.color);

	// Sphereの設定
	OBBCollider::Initialize();
	name_ = ColliderName::Enemy;
	obb_.halfSize = { 2.0f, 2.0f, 2.0f };

	// BaseEnemyの初期化
	BaseEnemy::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void CloseRangeEnemy::Update() {
	if (!player_) return;

	// タイマーを進める
	advanceTimer();

	/// ===Behavior遷移の実装=== ///
	if (behaviorRequest_) {
		// 振る舞いを変更
		behavior_ = behaviorRequest_.value();
		// 各振る舞いの初期化
		switch (behavior_) {
			// 移動
		case CloseRangeEnemy::Behavior::kMove:
			InitMove();
			break;
		case CloseRangeEnemy::Behavior::kAttack:
			InitAttack();
			break;
		}

		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 各振る舞いの更新
	switch (behavior_) {
	case CloseRangeEnemy::Behavior::kMove:
		Move();
		break;
	case CloseRangeEnemy::Behavior::kAttack:
		Attack();
		break;
	}

	

	// BaseEnemyの更新
	BaseEnemy::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void CloseRangeEnemy::Draw(BlendMode mode) {
	BaseEnemy::Draw(mode);
}

///-------------------------------------------/// 
/// 更新（ImGui）
///-------------------------------------------///
void CloseRangeEnemy::UpdateImGui() {
#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Translate", &baseInfo_.translate.x, 0.1f);
	ImGui::DragFloat4("Rotate", &baseInfo_.rotate.x, 0.1f);
	ImGui::DragFloat3("Direction", &moveInfo_.direction.x, 0.1f);
	ImGui::DragFloat3("Velocity", &baseInfo_.velocity.x, 0.1f);
	ImGui::DragFloat("Movetimer", &moveInfo_.timer, 0.1f);
	ImGui::Checkbox("IsWating", &moveInfo_.isWating);
	ImGui::End();
#endif // _DEBUG
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void CloseRangeEnemy::OnCollision(Collider* collider) {
	// Playerとの当たり判定
	if (collider->GetColliderName() == ColliderName::Player) {
		// Playerの突進に対しての衝突処理
		if (player_->GetStateFlag(actionType::kCharge)) {
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
void CloseRangeEnemy::InitMove() {
	// 移動範囲の中心を設定
	moveInfo_.rangeCenter = { attackInfo_.playerPos }; 
	// 初期速度をリセット
	baseInfo_.velocity = { 0.0f, 0.0f, 0.0f };
	// 動くまでの時間を設定
	moveInfo_.timer = 0.5f;
}
void CloseRangeEnemy::Move() {

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
void CloseRangeEnemy::PreparNextMove(const Vector3& vector) {
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
void CloseRangeEnemy::InitAttack() {
	// デバッグ用
	baseInfo_.color = { 0.0f, 1.0f, 0.0f, 1.0f }; // 攻撃中は赤色に変更
	
	// 移動ベクトルを0にリセット
	baseInfo_.velocity = {0.0f, 0.0f, 0.0f}; 

	// 待機時間を設定
	attackInfo_.timer = 1.0f;
}
void CloseRangeEnemy::Attack() {

	if (!attackInfo_.isAttack) { /// ===IsAttackがfalse=== ///
		// プレイヤー位置を取得
		attackInfo_.playerPos = player_->GetTranslate();

		// プレイヤー位置への方向ベクトル
		Vector3 dir = attackInfo_.playerPos - baseInfo_.translate;
		attackInfo_.direction = Normalize(dir); // 方向を保存

		// directionの方向に回転
		UpdateRotationTowards(attackInfo_.direction, 0.2f);

		// 少し待つ
		if (attackInfo_.timer <= 0.0f) { // タイマーが0以下
			// 移動ベクトルを設定
			baseInfo_.velocity = attackInfo_.direction * attackInfo_.moveSpeed;

			// 攻撃開始
			attackInfo_.isAttack = true;
		}

	} else { /// ===IsAttackがtrue=== ///

		// プレイヤーとの差を計算
		Vector3 toTarget = attackInfo_.playerPos - baseInfo_.translate;

		// 攻撃終了判定
		if (Length(toTarget) < 0.5f) { // 到達判定
			attackInfo_.isAttack = false; // 攻撃終了フラグをfalseに
			baseInfo_.velocity = { 0.0f, 0.0f, 0.0f }; // 移動ベクトルをリセット
			attackInfo_.timer = attackInfo_.interval; // クールダウン再設定

			baseInfo_.color = { 1.0f, 0.0f, 1.0f, 1.0f }; // 元の色に戻す（任意）

			/// ===Behaviorの変更=== ///
			behaviorRequest_ = Behavior::kMove; 
		}
	}
}

///-------------------------------------------/// 
/// 攻撃可能かチェック
///-------------------------------------------///
bool CloseRangeEnemy::CheckAttackable() {

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
void CloseRangeEnemy::UpdateRotationTowards(const Vector3& direction, float slerpT) {
	if (Length(direction) < 0.001f) return;

	// forward方向からターゲットクォータニオンを作成
	Quaternion targetRotation = Math::LookRotation(direction, Vector3(0.0f, 1.0f, 0.0f));

	// SLerp補間
	Quaternion result = Math::SLerp(baseInfo_.rotate, targetRotation, slerpT);
	baseInfo_.rotate = Normalize(result); // ★ 正規化でスケール崩れ防止
}

///-------------------------------------------/// 
/// 時間を進める
///-------------------------------------------///
void CloseRangeEnemy::advanceTimer() {
	// 移動移動用のタイマーをするメイル
	if (moveInfo_.timer > 0.0f) {
		moveInfo_.timer -= deltaTime_;
	}

	// 攻撃用のタイマーを進める
	if (attackInfo_.timer > 0.0f) {
		attackInfo_.timer -= deltaTime_;
	}
}