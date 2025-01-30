#pragma once
// ===Includ=== ///
#include <memory>

// Game
#include "application/3d/Model.h"
#include "application/3d/Camera.h"
#include "application/Manager/CameraManager.h"
#include "application/Game/Objects/Collider.h"


class Card : public Collider {
public:

	static Vector3 MakeTransform(const Vector3& vector, const Matrix4x4& matrix) {
		Vector3 resultVector;
		resultVector.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
		resultVector.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
		resultVector.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

		// wが0の場合の処理
		if (std::abs(w) < std::numeric_limits<float>::epsilon()) {
			w = 1.0f; // 安全なデフォルト値を設定
		}

		resultVector.x /= w;
		resultVector.y /= w;
		resultVector.z /= w;

		return resultVector;
	}


	enum class CardType
	{
	};

	enum class CardState
	{
		back,
		front,
		show,
		obtained,

		none,
	};

public:/// ===メンバ関数=== ///

	// 初期化
	void Initialize(const std::string& cardModel,int cardType,Vector3 position,std::shared_ptr<Camera> activeCamera);

	// 更新
	void Update(std::shared_ptr<Camera> activeCamera);

	// 描画
	void Draw();

	// ゲッター
	int GetCardType() { return cardType_; }

	CardState GetCurrentState() { return currentState_; }

	CardState GetRequestState() { return requestState_; }

	Vector2 GetScreenPosition() { return screenPosition; }

	AABB GetAABB() { return Collider::GetAABB(); }

	// セッター
	void RequestState(CardState state) { requestState_ = state; }

	void SetCurrentState(CardState state) { currentState_ = state; }

	void SetScale(Vector3 scale) { model->SetScale(scale); }

private:

	void CalculationScreenPosition(std::shared_ptr<Camera> activeCamera);

private:/// ===メンバ変数=== ///
	/// <summary>
	/// カード
	/// </summary>

	/// ===クラス=== ///

	// モデル
	std::unique_ptr<Model> model = nullptr;

	/// ===変数=== ///

	// カード番号
	int cardType_;

	float destinationRotateY = 0.0f;
	
	// カードの現在の状態
	CardState currentState_;

	// 
	CardState requestState_;

	Vector2 screenPosition = {};

	const float kDeltaTime_ = 1.0f / 60.0f;

	const float kShowTime_ = 1.0f;

	float elapsedTime_ = 0.0f;
};

