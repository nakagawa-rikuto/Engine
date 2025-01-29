#include "DebugScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"
#include "Engine/Service/Loader.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
DebugScene::~DebugScene() {
	// sprite
	sprite_.reset();
	// camera
	camera_.reset();
	camera2_.reset();
	// model
	model_.reset();
	// audio
	audio_->StopSound("fanfare");
	audio_->StopSound("clear");
	// Loader
	Loader::UnloadSound("fanfare");
	Loader::UnloadSound("clear");
	// Particle
	windParticle_.reset();
	explosionParticle_.reset();
	confettiParticle_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void DebugScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	/// ===読み込み=== ///
#pragma region 読み込み処理
	// 音声データの読み込み
	Loader::LoadWave("fanfare", "./Resource/BGM/fanfare.wav");
	// MP3を読み込むとものすごく重い
	//Loader::LoadMP3("clear", "./Resource/BGM/clear.mp3");

	// テクスチャの読み込み
	Loader::LoadTexture("uvChecker","./Resource/uvChecker.png");
	Loader::LoadTexture("monsterBall", "./Resource/monsterBall.png");

	// モデルの読み込み
	Loader::LoadModel("GlTF", ModelFileType::GLTF);
	Loader::LoadModel("MonsterBall");
	Loader::LoadModel("terrain");
	Loader::LoadModel("axis");
	Loader::LoadModel("plane");
	Loader::LoadModel("Particle");
	
#pragma endregion

	/// ===スプライトの初期化=== ///
#pragma region Spriteの初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("uvChecker");                   // 初期化(const std::string& spriteNameが必須)
	/* // テクスチャの使い方
	sprite->SetPosition(Vector2(0.0f, 0.0f));           // 場所の設定(初期値は0,0)
	sprite->SetRotation(0.0f);                          // 回転の設定(初期値は0.0);
	sprite->SetSize(Vector2(100.0f, 100.f));            // サイズの設定(初期値は640, 360)
	sprite->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));  // カラーの設定(初期値は1,1,1,1)
	sprite->SetAnchorPoint(Vector2(0.0f, 0.0f));        // アンカーポイントの設定(初期値は0,0)
	sprite->SetTextureSize(Vector2(64.0f, 64.0f));      // テクスチャサイズの設定(初期値は100.0f, 100.0f)
	*/
#pragma endregion

	/// ===モデルの初期化=== ///
#pragma region Modelの初期化
	model_ = std::make_unique<Model>();
	model_->Initialize("MonsterBall", LightType::PointLight);          // 初期化(const std::string& modelNameが必須)
	model2_ = std::make_unique<Model>();
	model2_->Initialize("terrain", LightType::PointLight);
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
#pragma endregion

	/// ===カメラの初期化=== ///
#pragma region Cameraの初期化
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
#pragma endregion

	/// ===ライト=== ///
#pragma region Lightの情報
#pragma endregion

	/// ===音=== ///
#pragma region Audio
	//audio_->PlayeSound("clear", false);
#pragma endregion

	/// ===Particle=== ///
#pragma region Particleの生成
	windParticle_ = std::make_shared<WindEmitter>();
	explosionParticle_ = std::make_shared<ExplosionEmitter>();
	confettiParticle_ = std::make_shared<ConfettiEmitter>();
#pragma endregion
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void DebugScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("DebugScene");
	if (ImGui::BeginCombo("Object", "Select")) {
		if (ImGui::Selectable("Sprite", isSetting_.Sprite)) {
			isSetting_.Sprite = true;
			isSetting_.Model = false;
			isSetting_.Particle1 = false;
			isSetting_.Particle2 = false;
			isSetting_.Particle3 = false;
		} else if (ImGui::Selectable("Model", isSetting_.Model)) {
			isSetting_.Sprite = false;
			isSetting_.Model = true;
			isSetting_.Particle1 = false;
			isSetting_.Particle2 = false;
			isSetting_.Particle3 = false;
		} else if (ImGui::Selectable("Particle1", isSetting_.Particle1)) {
			isSetting_.Sprite = false;
			isSetting_.Model = false;
			isSetting_.Particle1 = true;
			isSetting_.Particle2 = false;
			isSetting_.Particle3 = false;
		} else if (ImGui::Selectable("Particle2", isSetting_.Particle2)) {
			isSetting_.Sprite = false;
			isSetting_.Model = false;
			isSetting_.Particle1 = false;
			isSetting_.Particle2 = true;
			isSetting_.Particle3 = false;
		} else if (ImGui::Selectable("Particle3", isSetting_.Particle3)) {
			isSetting_.Sprite = false;
			isSetting_.Model = false;
			isSetting_.Particle1 = false;
			isSetting_.Particle2 = false;
			isSetting_.Particle3 = true;
		}
		ImGui::EndCombo();
	}
	/// ===Sprite=== ///
	if (isSetting_.Sprite) {
		if (!isDisplay_.Sprite && ImGui::Button("Draw")) {
			isDisplay_.Sprite = true;
		} else if (isDisplay_.Sprite && ImGui::Button("UnDraw")) {
			isDisplay_.Sprite = false;
		}
		if (!isImgui_.Sprite && ImGui::Button("Info")) {
			isImgui_.Sprite = true;
		} else if (isImgui_.Sprite && ImGui::Button("UnInfo")) {
			isImgui_.Sprite = false;
		}
		/// ===Info=== ///
		if (isImgui_.Sprite) {
			// Sprite
			ImGui::DragFloat2("Tranlate", &spriteTranslate_.x, 0.1f);
			ImGui::DragFloat("Rotate", &spriteRotate_, 0.1f);
			ImGui::DragFloat2("Size", &spriteSize_.x, 0.1f);
			ImGui::ColorEdit4("Color", &spriteColor_.x);
		}
	}
	/// ===Model=== ///
	if (isSetting_.Model) {
		if (!isDisplay_.Model && ImGui::Button("Draw")) {
			isDisplay_.Model = true;
		} else if (isDisplay_.Model && ImGui::Button("UnDraw")) {
			isDisplay_.Model = false;
		}
		if (!isImgui_.Model && ImGui::Button("Info")) {
			isImgui_.Model = true;
		} else if (isImgui_.Model && ImGui::Button("UnInfo")) {
			isImgui_.Model = false;
		}
		/// ===Info=== ///
		if (isImgui_.Model) {
			// Model
			ImGui::DragFloat3("Tranlate", &modelTranslate_.x, 0.1f);
			ImGui::DragFloat3("Rotate", &modelRotate_.x, 0.1f);
			ImGui::DragFloat3("Size", &modelScale_.x, 0.1f);
			ImGui::ColorEdit4("Color", &modelColor_.x);
			// Light
			ImGui::DragFloat("LightShininess", &light_.shininess, 0.01f);

			ImGui::ColorEdit4("LigthColor", &directional_.color.x);
			ImGui::DragFloat3("LightDirection", &directional_.direction.x, 0.01f);
			ImGui::DragFloat("lightIntensity", &directional_.intensity, 0.01f);
			
			ImGui::ColorEdit4("pointLightColor", &point_.color.x);
			ImGui::DragFloat3("pointLightPosition", &point_.position.x, 0.01f);
			ImGui::DragFloat("pointLightIntensity", &point_.intensity, 0.01f);
			ImGui::DragFloat("pointLightRadius", &point_.radius, 0.01f);
			ImGui::DragFloat("pointLightDecay", &point_.decay, 0.01f);
			
			ImGui::ColorEdit4("SpotLightColor", &spot_.color.x);
			ImGui::DragFloat3("spotLightPosition", &spot_.position.x, 0.01f);
			ImGui::DragFloat("SpotLightIntensity", &spot_.intensity, 0.01f);
			ImGui::DragFloat3("SpotLightDirection", &spot_.direction.x, 0.01f);
			ImGui::DragFloat("SpotLightDistance", &spot_.distance, 0.01f);
			ImGui::DragFloat("SpotLightDecay", &spot_.decay, 0.01f);
			ImGui::DragFloat("SpotLightCosAngle", &spot_.cosAngle, 0.01f);
			
		}
	}
	/// ===Particle1=== ///
	if (isSetting_.Particle1) {
		if (!isDisplay_.Particle1 && ImGui::Button("Draw")) {
			windParticle_->Initialze("Particle");
			windParticle_->SetTranslate(particleTranslate_);
			isDisplay_.Particle1 = true;
		} else if (isDisplay_.Particle1 && ImGui::Button("UnDraw")) {
			isDisplay_.Particle1 = false;
		}
		if (!isImgui_.Particle1 && ImGui::Button("Info")) {
			isImgui_.Particle1 = true;
		} else if (isImgui_.Particle1 && ImGui::Button("UnInfo")) {
			isImgui_.Particle1 = false;
		}
		/// ===Info=== ///
		if (isImgui_.Particle1) {
			// Particle
			ImGui::DragFloat3("Tranlate", &particleTranslate_.x, 0.1f);
		}
	}
	/// ===Particle2=== ///
	if (isSetting_.Particle2) {
		if (!isDisplay_.Particle2 && ImGui::Button("Draw")) {
			explosionParticle_->Initialze("Particle");
			explosionParticle_->SetTranslate(particleTranslate_);
			isDisplay_.Particle2 = true;
		} else if (isDisplay_.Particle2 && ImGui::Button("UnDraw")) {
			isDisplay_.Particle2 = false;
		}
		if (!isImgui_.Particle2 && ImGui::Button("Info")) {
			isImgui_.Particle2 = true;
		} else if (isImgui_.Particle2 && ImGui::Button("UnInfo")) {
			isImgui_.Particle2 = false;
		}
		/// ===Info=== ///
		if (isImgui_.Particle2) {
			// Particle
			ImGui::DragFloat3("Tranlate", &particleTranslate_.x, 0.1f);
		}
	}
	/// ===Particle3=== ///
	if (isSetting_.Particle3) {
		if (!isDisplay_.Particle3 && ImGui::Button("Draw")) {
			confettiParticle_->Initialze();
			confettiParticle_->SetTranslate(particleTranslate_);
			isDisplay_.Particle3 = true;
		} else if (isDisplay_.Particle3 && ImGui::Button("UnDraw")) {
			isDisplay_.Particle3 = false;
		}
		if (!isImgui_.Particle3 && ImGui::Button("Info")) {
			isImgui_.Particle3 = true;
		} else if (isImgui_.Particle3 && ImGui::Button("UnInfo")) {
			isImgui_.Particle3 = false;
		}
		/// ===Info=== ///
		if (isImgui_.Particle3) {
			// Particle
			ImGui::DragFloat3("Tranlate", &particleTranslate_.x, 0.1f);
		}
	}

	ImGui::End();

	/// ===Camera=== ///
	ImGui::Begin("Camera");
	ImGui::Checkbox("Flag", &SetCamera);
	ImGui::DragFloat3("Translate", &cameraPos.x, 0.1f);
	ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.1f);
	ImGui::End();
	/// ===Keybord=== ///
	ImGui::Begin("Keybord");
	ImGui::Text("WSADデカメラのポジションを移動");
	ImGui::End();
	/// ===Mouse=== ///
	ImGui::Begin("Mouse");
	ImGui::Checkbox("PushLeft", &PushLeft_);
	ImGui::Checkbox("TriggerRight", &TriggerRight_);
	ImGui::DragFloat2("MousePosition", &mousePosition_.x, 0.1f);
	ImGui::End();
	/// ===Controller=== ///
	ImGui::Begin("Controller");
	ImGui::End();
	/// ===Audio=== ///
	ImGui::Begin("Audio");
	ImGui::Checkbox("play", &playAudio);
	ImGui::DragFloat("Volume", &volume, 0.01f);
	ImGui::DragFloat("Ptich", &pitch, 0.01f);
	ImGui::End();

#endif // USE_IMGUI

	/// ===カメラの変更=== ///
#pragma region カメラの変更
	if (SetCamera) {
		cameraManager_->SetActiveCamera("Main2");
	} else {
		cameraManager_->SetActiveCamera("Main");
	}
#pragma endregion

	/// ===キーボード関連の処理=== ///
#pragma region キーボード関連の処理
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
#pragma endregion

	/// ===マウス関連の処理=== ///
#pragma region マウス関連の処理
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
#pragma endregion

	/// ===Audioのセット=== ///
#pragma region Audioのセット
	if (playAudio) {
		audio_->PlayeSound("fanfare", false);
		audio_->VolumeSound("fanfare", volume);
		audio_->PitchSound("fanfare", pitch);
	} else {
		audio_->StopSound("fanfare");
		audio_->StopSound("fanfare");
	}
#pragma endregion

	/// ===スプライトの更新=== ///
#pragma region スプライトの更新
	sprite_->SetPosition(spriteTranslate_);
	sprite_->SetRotation(spriteRotate_);
	sprite_->SetSize(spriteSize_);
	sprite_->SetColor(spriteColor_);
	sprite_->Update();
#pragma endregion

	/// ===モデルの更新=== ///
#pragma region モデルの更新
	/// ===回転処理=== ///
	if (isRotate) {
		modelRotate_.y += 0.1f;
		modelRotate_.x += 0.1f;
		modelRotate_.z -= 0.1f;
	}
	model_->SetTransform(modelTranslate_, modelRotate_, modelScale_);
	model_->SetColor(modelColor_);
	model_->SetShininess(light_);
	model_->SetDirctionalLightData(directional_);
	model2_->SetDirctionalLightData(directional_);
	model_->SetPointLightData(point_);
	model2_->SetPointLightData(point_);
	model_->SetSpotLightData(spot_);
	model2_->SetSpotLightData(spot_);
	model_->SetCamera(cameraManager_->GetActiveCamera().get());
	model2_->SetCamera(cameraManager_->GetActiveCamera().get());
	model_->Update();
	model2_->Update();
#pragma endregion

	/// ===Particle=== ///
#pragma region Particle
	windParticle_->Update();
	explosionParticle_->Update();
	confettiParticle_->Update();
#pragma endregion

	/// ===カメラの更新=== ///
#pragma region カメラの更新
	camera_->SetRotate(cameraRotate);
	camera_->SetTranslate(cameraPos);
	// 全てのカメラの更新
	cameraManager_->UpdateAllCameras();
#pragma endregion
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void DebugScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
	/// ===Model=== ///
	if (isDisplay_.Model) {
		model_->Draw(); // BlendMode変更可能 model_->Draw(BlendMode::kBlendModeAdd);
		model2_->Draw();
	}
	/// ===Particle=== ///
	if (isDisplay_.Particle1) {
		windParticle_->Draw();
	} 
	if (isDisplay_.Particle2) {
		explosionParticle_->Draw();
	} 
	if (isDisplay_.Particle3) {
		confettiParticle_->Draw();
	}
	
#pragma endregion

#pragma region 前景スプライト描画
	/// ===Sprite=== ///
	if (isDisplay_.Sprite) {
		sprite_->Draw(); // BlendMode変更可　sprite->Draw(BlendMode::kBlendModeAdd);  
	}
#pragma endregion
}