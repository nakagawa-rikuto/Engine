#include "Game/Scene/GameScene.h"
// SceneManager
#include "Game/Manager/SceneManager.h"
#include "Engine/Input/Input.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	const std::string& uvTexture = "./Resource/uvChecker.png";
	Mii::LoadTexture(uvTexture);

	/*const std::string& white = "./Resource/white1x1.png";
	Mii::LoadTexture(white);*/

	object_.SetPosition(Vector2(100.0f, 100.0f));

	leftCommand_ = std::make_unique<LeftCommand>();
	rightCommand_ = std::make_unique<RightCommand>();

	box_ = std::make_unique<Sprite>();
	box_->Initialize();
	box_->SetTexture(uvTexture);
	box_->SetSize(Vector2(30.0f, 30.0f));
	box_->SetPosition(object_.GetPosition());
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef _DEBUG
	ImGui::Begin("GameScene");
	ImGui::End();
#endif // DEBUG

	if (input_->PushKey(DIK_D)) {
		rightCommand_->Execute(object_);
	} else if (input_->PushKey(DIK_A)) {
		leftCommand_->Execute(object_);
	}

	box_->SetPosition(object_.GetPosition());
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
#pragma endregion

#pragma region 前景スプライト描画

	box_->Draw();

#pragma endregion
}
