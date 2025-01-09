#include "Card.h"

void Card::Initialize(const std::string& cardModel,int cardType, Vector3 position, std::shared_ptr<Camera> activeCamera)
{
	// モデルの生成と初期化
	model = std::make_unique<Model>();
	model->Initialize(cardModel);
	model->SetPosition(position);
	model->SetCamera(activeCamera.get());

	// メンバ変数にカードの番号を記録
	cardType_ = cardType;

	// カードの状態を裏で初期化
	currentState_ = CardState::back;
	requestState_ = CardState::none;
}

void Card::Update(std::shared_ptr<Camera> activeCamera) {

	// 各状態の角度Y
	float rotateyTable[] = { 0.0f,3.14f ,3.14f,3.14f };


	float destinationRotateY = 0.0f;

	//// 状態に合わせた目標角度を設定
	//if (currentState_ == CardState::front && requestState_ == CardState::back) {
	//	
	//	destinationRotateY = rotateyTable[static_cast<int>(CardState::back)];

	//}
	//else if (currentState_ == CardState::back && requestState_ == CardState::front) {

	//	destinationRotateY = rotateyTable[static_cast<int>(CardState::front)];

	//}
	//else if (requestState_ == CardState::none) {

	//	destinationRotateY = rotateyTable[static_cast<int>(currentState_)];

	//}

	//
	if (currentState_ != requestState_)
	{
		if (requestState_ == CardState::none || requestState_ == CardState::obtained) {
			
		}
		else {
			destinationRotateY = rotateyTable[static_cast<int>(currentState_)];
		}
	}

	// 現在の角度を取得
	Vector3 rotate = model->GetRotate();

	// 差を計算
	float diffrotate = destinationRotateY - rotate.y;

	rotate.y += diffrotate * 0.1f;

	if (diffrotate <= 0.1f && diffrotate >= 0.0f ||
		diffrotate >= -0.1f && diffrotate <= 0.0f)
	{
		if (requestState_ != CardState::none)
		{
			currentState_ = requestState_;
			requestState_ = CardState::none;
			
			rotate.y = rotateyTable[static_cast<int>(currentState_)];
		}
	}

	// 移動後の数値を代入
	model->SetRotate(rotate);

	// 有効なカメラの更新
	model->SetCamera(activeCamera.get());

	// モデルの更新
	model->Update();
}

void Card::Draw()
{
	// モデルの描画

	// 未取得の場合
	if (currentState_ != CardState::obtained)
	{
		model->Draw();
	}


}
