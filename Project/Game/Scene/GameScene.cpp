#include "Game/Scene/GameScene.h"
// SceneManager
#include "Game/Manager/SceneManager.h"

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

	Mii::LoadTexture("./Resource/monsterBall.png");
	Mii::LoadTexture("./Resource/uvChecker.png");
	Mii::LoadTexture("./Resource/checkerBoard.png");

	player_ = std::make_unique<Player>();
	player_->Initialzie();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
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

	/// ===当たり判定=== ///
	

	player_->Update();
	enemy_->Update();
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
	enemy_->Draw();
	player_->Draw();
#pragma endregion
}