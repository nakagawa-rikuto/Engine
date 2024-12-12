#include "Game/Scene/GameScene.h"
// SceneManager
#include "Game/Manager/SceneManager.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	sprite_.reset();
	camera_.reset();
	camera2_.reset();
	model_.reset();
	Mii::UnloadSound("fanfare");
	Mii::UnloadSound("clear");
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// 音声データの読み込み
	Mii::LoadSound("fanfare", "./Resource/BGM/fanfare.wav", false);
	// MP3を読み込むとものすごく重い
	//Mii::LoadSound("clear", "./Resource/BGM/clear.mp3", true);

	//// テクスチャの読み込み
	const std::string& uvTexture = "./Resource/uvChecker.png";
	Mii::LoadTexture(uvTexture);
	const std::string& monsterBall = "./Resource/monsterBall.png";
	Mii::LoadTexture(monsterBall);

	// モデルの読み込み
	const std::string& planeModel = "plane";
	Mii::LoadModel(planeModel);
	const std::string& axisModel = "axis";
	Mii::LoadModel(axisModel);

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize();                              // BlendMode変更可　sprite->Initialize(BlendMode::kBlendModeAdd);  
	sprite_->SetTexture(uvTexture);                     // テクスチャの設定(これがないと描画できない)
	/* // テクスチャの使い方
	sprite->SetPosition(Vector2(0.0f, 0.0f));          // 場所の設定(初期値は0,0)
	sprite->SetRotation(0.0f);                         // 回転の設定(初期値は0.0);
	sprite->SetSize(Vector2(100.0f, 100.f));           // サイズの設定(初期値は640, 360)
	sprite->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)); // カラーの設定(初期値は1,1,1,1)
	sprite->SetAnchorPoint(Vector2(0.0f, 0.0f));       // アンカーポイントの設定(初期値は0,0)
	sprite->SetTextureSize(Vector2(64.0f, 64.0f));     // テクスチャサイズの設定(初期値は100.0f, 100.0f)
	*/

	// モデル
	model_ = std::make_unique<Model>();
	model_->Initialize(axisModel);

	/* // モデルの使い方
	model->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	model->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	model->SetScale(Vector3(0.0f, 0.0f, 0.0f));
	model->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	model->SetCamera(cameraManager_->GetActiveCamera().get());
	*/

	//Camera
	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });

	camera2_ = std::make_shared<Camera>();
	camera2_->Initialize();
	camera2_->SetTranslate({ 0.0f, 0.0f, -50.0f });
	camera2_->SetRotate({ 0.0f, 0.0f, 0.0f });

	cameraManager_->Add("Main", camera_);
	cameraManager_->Add("Main2", camera2_);

	//Mii::PlayeSound("clear", false);
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

#ifdef _DEBUG
	ImGui::Begin("model");
	ImGui::Checkbox("RotateFlag", &isRotate);
	ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
	ImGui::End();

	ImGui::Begin("sprite");
	ImGui::DragFloat2("size", &size.x, 0.1f);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::Checkbox("Flag", &SetCamera);
	ImGui::DragFloat3("Translate", &cameraPos.x, 0.1f);
	ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.1f);
	ImGui::End();

	ImGui::Begin("Audio");
	ImGui::Checkbox("play", &playAudio);
	ImGui::DragFloat("Volume", &volume, 0.01f);
	ImGui::DragFloat("Ptich", &pitch, 0.01f);
	ImGui::End();
#endif // _DEBUG

	if (SetCamera) {
		cameraManager_->SetActiveCamera("Main");
	} else {
		cameraManager_->SetActiveCamera("Main2");
	}

	if (playAudio) {
		Mii::PlayeSound("fanfare", false);
		Mii::VolumeSound("fanfare", volume);
		Mii::PitchSound("fanfare", pitch);
	} else {
		Mii::StopSound("fanfare");
	}

	if (isRotate) {
		rotate.y += 0.1f;
		rotate.x += 0.1f;
		rotate.z -= 0.1f;
	}

	model_->SetRotate(rotate);
	model_->SetCamera(cameraManager_->GetActiveCamera().get());

	//sprite->SetSize(size);

	camera_->SetRotate(cameraRotate);
	camera_->SetTranslate(cameraPos);

	cameraManager_->UpdateAllCameras();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
	// Modelの描画
	model_->Draw();
#pragma endregion

#pragma region 前景スプライト描画
	// Spriteの描画
	sprite_->Draw();
#pragma endregion
}
