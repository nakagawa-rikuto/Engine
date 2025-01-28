#include "Card.h"
#include "Math/MatrixMath.h"

void Card::Initialize(const std::string& cardModel,int cardType, Vector3 position, std::shared_ptr<Camera> activeCamera)
{

	// AABBのサイズ調整
	Collider::Initialize({ 1.0f,1.0f,2.0f });

	// モデルの生成と初期化
	model = std::make_unique<Model>();
	model->Initialize(cardModel);
	model->SetPosition(position);
	model->SetCamera(activeCamera.get());

	// メンバ変数にカードの番号を記録
	cardType_ = cardType;

	// カードの状態を初期化
	currentState_ = CardState::back;
	requestState_ = CardState::none;	
}

void Card::Update(std::shared_ptr<Camera> activeCamera) {

	Collider::Update(model->GetPosition());

	if (currentState_ == CardState::show && elapsedTime_ < kShowTime_) {

		elapsedTime_ += kDeltaTime_;

		if (elapsedTime_ > kShowTime_) {

		}
		else {
			return;
		}
	}

	// 各状態の角度Y
	float rotateyTable[] = { 0.0f,3.14f,3.14f};

	// 現在の状態とリクエストの状態が違う場合
	if (currentState_ != requestState_) {

		// リクエストの状態が裏向きの時
		if (requestState_ == CardState::back) {
			destinationRotateY = rotateyTable[static_cast<int>(requestState_)];
		}

		// リクエストの状態が表向きの時
		else if (requestState_ == CardState::front) {
			destinationRotateY = rotateyTable[static_cast<int>(requestState_)];
		}

		else if (requestState_ == CardState::show) {
			destinationRotateY = rotateyTable[static_cast<int>(requestState_)];
		}

		// 現在の角度を取得
		Vector3 rotate = model->GetRotate();

		// 角度差を計算
		float diffrotate = destinationRotateY - rotate.y;

		// 角度差が小さい場合　
		if (diffrotate <= 0.1f && diffrotate >= 0.0f ||
			diffrotate >= -0.1f && diffrotate <= 0.0f)
		{
			// 細かい数値を調整
			if (requestState_ != CardState::none)
			{
				rotate.y = rotateyTable[static_cast<int>(requestState_)];

				if (currentState_ == CardState::show) {
					elapsedTime_ = 0.0f;
				}

				currentState_ = requestState_;
				requestState_ = CardState::none;

			}
		}
		else {
			rotate.y += diffrotate * 0.1f;
		}

		// 移動後の数値を代入
		model->SetRotate(rotate);
	}

	// 有効なカメラの更新
	model->SetCamera(activeCamera.get());

	// モデルの更新
	model->Update();

	CalculationScreenPosition(activeCamera);
}

void Card::Draw()
{
	// モデルの描画

	// 未取得の場合に描画
	if (currentState_ != CardState::obtained)
	{
		model->Draw();
	}
}

void Card::CalculationScreenPosition(std::shared_ptr<Camera> activeCamera)
{
	Matrix4x4 wMat = MakeAffineMatrix(model->GetScale(), model->GetRotate(), model->GetPosition());

	Vector3 wPos = { wMat.m[3][0],wMat.m[3][1] ,wMat.m[3][2] };

	Matrix4x4 matViewport = MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);

	Matrix4x4 matViewProjectionViewPort = Multiply(activeCamera->GetViewProjectionMatrix(), matViewport);

	wPos = MakeTransform(wPos, matViewProjectionViewPort);

	screenPosition = { wPos.x,wPos.y };
}
