#include "GameScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"
// Engine
#include "Engine/Service/Loader.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	model_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// モデルの読み込み
	Loader::LoadModel("MonsterBall");

	// モデルの生成
	model_ = std::make_unique<Model>();
	model_->Initialize("MonsterBall", LightType::HalfLambert);

	// Light
	lightDirection_ = { 0.0f, -1.0f, 0.0f };
	lightIntensity_ = 1.0f;
	lightColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	lightShininess_ = 48.5f;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("GameScene");

	// Model
	ImGui::DragFloat3("Tranlate", &modelTranslate_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &modelRotate_.x, 0.1f);
	ImGui::DragFloat3("Size", &modelScale_.x, 0.1f);
	ImGui::ColorEdit4("Color", &modelColor_.x);
	// Light
	ImGui::DragFloat3("LightDirection", &lightDirection_.x, 0.01f);
	ImGui::DragFloat("lightIntensity", &lightIntensity_, 0.01f);
	ImGui::DragFloat("LightShininess", &lightShininess_, 0.01f);
	ImGui::ColorEdit4("LigthColor", &lightColor_.x);

	ImGui::End();
#endif // USE_IMGUI

	/// ===モデルの更新=== ///
	model_->SetPosition(modelTranslate_);
	model_->SetRotate(modelRotate_);
	model_->SetScale(modelScale_);
	model_->SetColor(modelColor_);
	model_->SetLightColor(lightColor_);
	model_->SetLightDirection(lightDirection_);
	model_->SetLightIntensity(lightIntensity_);
	model_->SetLightShininess(lightShininess_);
	model_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
	model_->Draw();
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}