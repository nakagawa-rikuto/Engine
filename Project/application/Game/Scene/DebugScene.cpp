#include "DebugScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
DebugScene::~DebugScene() {
	sprite_.reset();
	camera_.reset();
	camera2_.reset();
	model_.reset();
	audio_->StopSound("fanfare");
	audio_->StopSound("clear");
	Loader_->UnloadSound("fanfare");
	Loader_->UnloadSound("clear");
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void DebugScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	/// ===読み込み=== ///
	// 音声データの読み込み
	Loader_->LoadWave("fanfare", "./Resource/BGM/fanfare.wav");
	// MP3を読み込むとものすごく重い
	//load_->LoadMP3("clear", "./Resource/BGM/clear.mp3");

	// テクスチャの読み込み
	const std::string& uvTexture = "./Resource/uvChecker.png";
	Loader_->LoadTexture(uvTexture);
	const std::string& monsterBall = "./Resource/monsterBall.png";
	Loader_->LoadTexture(monsterBall);

	// モデルの読み込み
	const std::string& planeModel = "MonsterBall";
	Loader_->LoadModel(planeModel);
	const std::string& axisModel = "axis";
	Loader_->LoadModel(axisModel);
	Loader_->LoadModel("plane");

	/// ===スプライトの初期化=== ///
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(uvTexture);                   // 初期化(const std::string& spriteNameが必須)
	/* // テクスチャの使い方
	sprite->SetPosition(Vector2(0.0f, 0.0f));           // 場所の設定(初期値は0,0)
	sprite->SetRotation(0.0f);                          // 回転の設定(初期値は0.0);
	sprite->SetSize(Vector2(100.0f, 100.f));            // サイズの設定(初期値は640, 360)
	sprite->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));  // カラーの設定(初期値は1,1,1,1)
	sprite->SetAnchorPoint(Vector2(0.0f, 0.0f));        // アンカーポイントの設定(初期値は0,0)
	sprite->SetTextureSize(Vector2(64.0f, 64.0f));      // テクスチャサイズの設定(初期値は100.0f, 100.0f)
	*/

	/// ===モデル=== ///
	model_ = std::make_unique<Model>();
	model_->Initialize(planeModel, LightType::HalfLambert);          // 初期化(const std::string& modelNameが必須)
	/* // モデルの使い方                        
	model_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));              // 座標の設定(初期値は {0.0f, 0.0f, 0.0f} )
	model_->SetRotate(Vector3(0.0f, 0.0f, 0.0f));                // 回転の設定(初期値は {0.0f, 0.0f, 0.0f} )
	model_->SetScale(Vector3(0.0f, 0.0f, 0.0f));                 // スケールの設定(初期値は {1.0f, 1.0f, 1.0f} )
	model_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));           // カラーの設定(初期値は {1.0f, 1.0f, 1.0f, 1.0f} )
	model_->SetLightDirection(Vector3(0.0f, -1.0f, 0.0f));       // Lightの向き設定(初期値は {0.0f, -1.0f, 0.0f})
	model_->SetLightIntensity(1.0f);                             // Lightの明るさの設定(初期値は {1.0f})
	model_->SetLightColor(Vector4(1.0f, 1.0f, 1.0f, 1.0));       // Lightカラーの設定(初期値は {1.0f, 1.0f, 1.0f, 1.0f})
	model_->SetLightShininess(0.27f);                            // 光沢度の設定(初期値は0.27f)
	model_->SetCamera(cameraManager_->GetActiveCamera().get());  // カメラの設定(初期値は {{1.0f, 1.0f,1.0f}, {0.3f, 0.0f, 0.0f}, {0.0f, 4.0f, -10.0f}};)
	*/

	/// ===Camera=== ///
	// カメラ1
	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
	// カメラ2
	camera2_ = std::make_shared<Camera>();
	camera2_->Initialize();
	camera2_->SetTranslate({ 0.0f, 0.0f, -30.0f });
	camera2_->SetRotate({ 0.0f, 0.0f, 0.0f });
	// カメラマネージャにカメラを追加
	cameraManager_->Add("Main", camera_);
	cameraManager_->Add("Main2", camera2_);

	/// ===Light=== ///
	lightDirection_ = { 0.0f, -1.0f, 0.0f };
	lightIntensity_ = 1.0f;
	lightColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	/// ===Audio=== ///
	//audio_->PlayeSound("clear", false);

	/// ===Particle=== ///
	windParticle_ = std::make_shared<WindEmitter>();
	windParticle_->Initialze();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void DebugScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("DebugScene");
	ImGui::End();
#endif // USE_IMGUI

#ifdef USE_IMGUI
	ImGui::Begin("model");
	ImGui::Checkbox("RotateFlag", &isRotate);
	ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &scale_.x, 0.01f);
	ImGui::ColorEdit4("LigthColor", &lightColor_.x);
	ImGui::DragFloat3("LightDirection", &lightDirection_.x, 0.01f);
	ImGui::DragFloat("lightIntensity", &lightIntensity_, 0.01f);
	ImGui::DragFloat("LightShininess", &lightShininess_, 0.01f);
	ImGui::End();

	ImGui::Begin("sprite");
	ImGui::DragFloat2("size", &size.x, 0.1f);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::Checkbox("Flag", &SetCamera);
	ImGui::DragFloat3("Translate", &cameraPos.x, 0.1f);
	ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.1f);
	ImGui::End();

	ImGui::Begin("Keybord");
	ImGui::Text("WSADデカメラのポジションを移動");
	ImGui::End();

	ImGui::Begin("Mouse");
	ImGui::Checkbox("PushLeft", &PushLeft_);
	ImGui::Checkbox("TriggerRight", &TriggerRight_);
	ImGui::DragFloat2("MousePosition", &mousePosition_.x, 0.1f);
	ImGui::End();

	ImGui::Begin("Controller");
	ImGui::End();

	ImGui::Begin("Audio");
	ImGui::Checkbox("play", &playAudio);
	ImGui::DragFloat("Volume", &volume, 0.01f);
	ImGui::DragFloat("Ptich", &pitch, 0.01f);
	ImGui::End();
#endif // USE_IMGUI

	/// ===カメラの変更=== ///
	if (SetCamera) {
		cameraManager_->SetActiveCamera("Main2");
	} else {
		cameraManager_->SetActiveCamera("Main");
	}

	/// ===キーボード関連の処理=== ///
	if (Mii::PushKey(DIK_D)) {
		cameraPos.x += 0.01f;
	} else if (Mii::PushKey(DIK_A)) {
		cameraPos.x -= 0.01f;
	}
	if (Mii::PushKey(DIK_W)) {
		cameraPos.y += 0.01f;
	} else if (Mii::PushKey(DIK_S)) {
		cameraPos.y -= 0.01f;
	}
	if (Mii::PushKey(DIK_UP)) {
		cameraPos.z += 0.01f;
	} else if (Mii::PushKey(DIK_DOWN)) {
		cameraPos.z -= 0.01f;
	}

	/// ===マウス関連の処理=== ///
	if (Mii::PushMouse(MouseButtonType::Left)) {
		PushLeft_ = true;
	} else {
		PushLeft_ = false;
	}
	if (Mii::TriggerMouse(MouseButtonType::Right)) {
		if (TriggerRight_) {
			TriggerRight_ = false;
		} else {
			TriggerRight_ = true;
		}
	}
	mousePosition_.x = static_cast<float>(Mii::GetMousePosition().x);
	mousePosition_.y = static_cast<float>(Mii::GetMousePosition().y);

	/// ===Audioのセット=== ///
	if (playAudio) {
		audio_->PlayeSound("fanfare", false);
		audio_->VolumeSound("fanfare", volume);
		audio_->PitchSound("fanfare", pitch);
	} else {
		audio_->StopSound("fanfare");
		audio_->StopSound("fanfare");
	}

	// 回転処理
	if (isRotate) {
		rotate.y += 0.1f;
		rotate.x += 0.1f;
		rotate.z -= 0.1f;
	}

	/// ===スプライトの更新=== ///
	//sprite->SetSize(size);
	sprite_->Update();

	/// ===Particle=== ///
	windParticle_->Update();

	/// ===モデルの更新=== ///
	model_->SetRotate(rotate);
	model_->SetScale(scale_);
	model_->SetLightColor(lightColor_);
	model_->SetLightDirection(lightDirection_);
	model_->SetLightIntensity(lightIntensity_);
	model_->SetLightShininess(lightShininess_);
	model_->SetCamera(cameraManager_->GetActiveCamera().get());
	model_->Update();

	/// ===カメラの更新=== ///
	camera_->SetRotate(cameraRotate);
	camera_->SetTranslate(cameraPos);
	// 全てのカメラの更新
	cameraManager_->UpdateAllCameras();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void DebugScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
	// Modelの描画
	//model_->Draw(); // BlendMode変更可能 model_->Draw(BlendMode::kBlendModeAdd)
	windParticle_->Draw();
#pragma endregion

#pragma region 前景スプライト描画
	// Spriteの描画
	//sprite_->Draw(); // BlendMode変更可　sprite->Draw(BlendMode::kBlendModeAdd);  
#pragma endregion
}