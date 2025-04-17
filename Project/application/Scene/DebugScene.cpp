#include "DebugScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// Service
#include "Engine/System/Service/Input.h"
#include "Engine/System/Service/Audio.h"
#include "Engine/System/Service/Particle.h"

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
	model2_.reset();
	modelLight_.reset();
	// animationModel
	animationModel_.reset();

	// ISceneのデストラクタ
	IScene::~IScene();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void DebugScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

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
	sprite_->Update();
#pragma endregion

	/// ===モデルの初期化=== ///
#pragma region Modelの初期化
	model_ = std::make_unique<Object3d>();
	model_->Init(ObjectType::Model, "MonsterBall", LightType::PointLight);          // 初期化(const std::string& modelNameが必須)
	model2_ = std::make_unique<Object3d>();
	model2_->Init(ObjectType::Model, "terrain", LightType::PointLight);
	modelLight_ = std::make_unique<Object3d>();
	modelLight_->Init(ObjectType::Model, "Particle");
	// modelLight_->SetTransform({ spot_.position }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
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

	// 球
	sky_ = std::make_unique<Object3d>();
	sky_->Init(ObjectType::Model, "sky", LightType::HalfLambert);
	sky_->SetTranslate({ 0.0f, 0.0f, 0.0f });
	cloud_ = std::make_unique<Object3d>();
	cloud_->Init(ObjectType::Model, "cloud", LightType::HalfLambert);
	cloud_->SetTranslate({ 0.0f, 0.0f, 0.0f });

	// ポイントライトの位置と半径の変更
	light_.point.position = { 0.0f, 0.0f, 99.0f };
	light_.point.radius = 500.0f;

	// 一回更新をかける
	model_->Update();
	model2_->Update();
	modelLight_->Update();
	sky_->Update();
	cloud_->Update();
#pragma endregion

	/// ===アニメーションモデルの初期化=== ///
#pragma region AnimationModelの初期化
	animationModel_ = std::make_unique<Object3d>();
	animationModel_->Init(ObjectType::AnimationModel, "human", LightType::Lambert);
	// アニメーションを登録しないとアニメーションが再生されない
	animationModel_->SetAnimation("Armature|mixamo.com|Layer0");
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
	cameraManager_->Add("Debug", camera_);
	cameraManager_->Add("Debug2", camera2_);
#pragma endregion

	/// ===ライト=== ///
#pragma region Lightの情報
#pragma endregion

	/// ===音=== ///
#pragma region Audio
	//audio_->PlayeSound("clear", false);
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
			if (!lightType_.Lambert && ImGui::Button("Lambert")) {
				model_->SetLight(LightType::Lambert);
				model2_->SetLight(LightType::Lambert);
				lightType_.Lambert = true;
				lightType_.HalfLambert = false;
				lightType_.PointLight = false;
				lightType_.SpotLight = false;
				lightType_.None = false;
			}
			if (!lightType_.HalfLambert && ImGui::Button("HalfLambert")) {
				model_->SetLight(LightType::HalfLambert);
				model2_->SetLight(LightType::HalfLambert);
				lightType_.Lambert = false;
				lightType_.HalfLambert = true;
				lightType_.PointLight = false;
				lightType_.SpotLight = false;
				lightType_.None = false;
			}
			if (!lightType_.PointLight && ImGui::Button("PointLight")) {
				model_->SetLight(LightType::PointLight);
				model2_->SetLight(LightType::PointLight);
				lightType_.Lambert = false;
				lightType_.HalfLambert = false;
				lightType_.PointLight = true;
				lightType_.SpotLight = false;
				lightType_.None = false;
			}
			if (!lightType_.SpotLight && ImGui::Button("SpotLight")) {
				model_->SetLight(LightType::SpotLight);
				model2_->SetLight(LightType::SpotLight);
				lightType_.Lambert = false;
				lightType_.HalfLambert = false;
				lightType_.PointLight = false;
				lightType_.SpotLight = true;
				lightType_.None = false;
			}
			if (!lightType_.None && ImGui::Button("None")) {
				model_->SetLight(LightType::None);
				model2_->SetLight(LightType::None);
				lightType_.Lambert = false;
				lightType_.HalfLambert = false;
				lightType_.PointLight = false;
				lightType_.SpotLight = false;
				lightType_.None = true;
			}
			if (lightType_.Lambert) {
				// Shiniess
				ImGui::DragFloat("LightShininess", &light_.shininess, 0.01f);
				// DirectionalLight
				ImGui::Text("DirectionalLight");
				ImGui::ColorEdit4("LigthColor", &light_.directional.color.x);
				ImGui::DragFloat3("LightDirection", &light_.directional.direction.x, 0.01f);
				ImGui::DragFloat("lightIntensity", &light_.directional.intensity, 0.01f);
			} else if (lightType_.HalfLambert) {
				// Shiniess
				ImGui::DragFloat("LightShininess", &light_.shininess, 0.01f);
				// DirectionalLight
				ImGui::Text("DirectionalLight");
				ImGui::ColorEdit4("LigthColor", &light_.directional.color.x);
				ImGui::DragFloat3("LightDirection", &light_.directional.direction.x, 0.01f);
				ImGui::DragFloat("lightIntensity", &light_.directional.intensity, 0.01f);
			} else if (lightType_.PointLight) {
				// Shiniess
				ImGui::DragFloat("LightShininess", &light_.shininess, 0.01f);
				// PointLight
				ImGui::Text("PointLight");
				ImGui::ColorEdit4("pointLightColor", &light_.point.color.x);
				ImGui::DragFloat3("pointLightPosition", &light_.point.position.x, 0.01f);
				ImGui::DragFloat("pointLightIntensity", &light_.point.intensity, 0.01f);
				ImGui::DragFloat("pointLightRadius", &light_.point.radius, 0.01f);
				ImGui::DragFloat("pointLightDecay", &light_.point.decay, 0.01f);
			} else if (lightType_.SpotLight) {
				// Shiniess
				ImGui::DragFloat("LightShininess", &light_.shininess, 0.01f);
				// SpotLight
				ImGui::Text("SpotLight");
				ImGui::ColorEdit4("SpotLightColor", &light_.spot.color.x);
				ImGui::DragFloat3("spotLightPosition", &light_.spot.position.x, 0.01f);
				ImGui::DragFloat("SpotLightIntensity", &light_.spot.intensity, 0.01f);
				ImGui::DragFloat3("SpotLightDirection", &light_.spot.direction.x, 0.01f);
				ImGui::DragFloat("SpotLightDistance", &light_.spot.distance, 0.01f);
				ImGui::DragFloat("SpotLightDecay", &light_.spot.decay, 0.01f);
				ImGui::DragFloat("SpotLightCosAngle", &light_.spot.cosAngle, 0.01f);
			}
		}
	}
	/// ===Particle1=== ///
	if (isSetting_.Particle1) {
		if (!isDisplay_.Particle1 && ImGui::Button("Draw")) {
			Particle::Emit("Wind", particleTranslate_);
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
			Particle::Emit("Explosion", particleTranslate_);
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
			Particle::Emit("Confetti", particleTranslate_);
			Particle::SetTexture("Confetti", "monsterBall");
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
	/*ImGui::Begin("Keybord");
	ImGui::Text("WSADデカメラのポジションを移動");
	ImGui::End();*/
	/// ===Mouse=== ///
	/*ImGui::Begin("Mouse");
	ImGui::Checkbox("PushLeft", &PushLeft_);
	ImGui::Checkbox("TriggerRight", &TriggerRight_);
	ImGui::DragFloat2("MousePosition", &mousePosition_.x, 0.1f);
	ImGui::End();*/
	/// ===Controller=== ///
	/*ImGui::Begin("Controller");
	ImGui::End();*/
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
		cameraManager_->SetActiveCamera("Debug2");
	} else {
		cameraManager_->SetActiveCamera("Debug");
	}
#pragma endregion

	/// ===キーボード関連の処理=== ///
#pragma region キーボード関連の処理
	if (Input::PushKey(DIK_D)) {
		cameraPos.x += 0.01f;
	} else if (Input::PushKey(DIK_A)) {
		cameraPos.x -= 0.01f;
	}
	if (Input::PushKey(DIK_W)) {
		cameraPos.y += 0.01f;
	} else if (Input::PushKey(DIK_S)) {
		cameraPos.y -= 0.01f;
	}
	if (Input::PushKey(DIK_UP)) {
		cameraPos.z += 0.01f;
	} else if (Input::PushKey(DIK_DOWN)) {
		cameraPos.z -= 0.01f;
	}
#pragma endregion

	/// ===マウス関連の処理=== ///
#pragma region マウス関連の処理
	if (Input::PushMouse(MouseButtonType::Left)) {
		PushLeft_ = true;
	} else {
		PushLeft_ = false;
	}
	if (Input::TriggerMouse(MouseButtonType::Right)) {
		if (TriggerRight_) {
			TriggerRight_ = false;
		} else {
			TriggerRight_ = true;
		}
	}
	mousePosition_.x = static_cast<float>(Input::GetMousePosition().x);
	mousePosition_.y = static_cast<float>(Input::GetMousePosition().y);
#pragma endregion

	/// ===コントローラーの処理=== ///
#pragma region コントローラの処理

	// スティックの入力処理を取得
	StickState leftStick = Input::GetLeftStickState(0);
	StickState rightStick = Input::GetRightStickState(0);

	// モデルの移動処理
	modelTranslate_.x += leftStick.x * 0.01f;
	modelTranslate_.y += leftStick.y * 0.01f;

	// カメラの移動処理
	cameraPos.x += rightStick.x * 0.01f;
	cameraPos.y += rightStick.y * 0.01f;

#pragma endregion

	/// ===Audioのセット=== ///
#pragma region Audioのセット
	if (playAudio) {
		Audio::PlayeSound("fanfare", false);
		Audio::VolumeSound("fanfare", volume);
		Audio::PitchSound("fanfare", pitch);
	} else {
		Audio::StopSound("fanfare");
		Audio::StopSound("fanfare");
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
	model_->SetTranslate(modelTranslate_);
	model_->SetRotate(modelRotate_);
	model_->SetScale(modelScale_);
	model_->SetColor(modelColor_);
	model_->SetLightData(light_);
	model_->SetCamera(cameraManager_->GetActiveCamera().get());
	model_->Update();

	model2_->SetLightData(light_);
	model2_->SetCamera(cameraManager_->GetActiveCamera().get());
	model2_->Update();

	modelLight_->SetTranslate(light_.point.position);
	modelLight_->SetCamera(cameraManager_->GetActiveCamera().get());
	modelLight_->Update();


	sky_->SetLightData(light_);
	sky_->SetCamera(cameraManager_->GetActiveCamera().get());
	sky_->Update();

	cloud_->SetLightData(light_);
	cloud_->SetCamera(cameraManager_->GetActiveCamera().get());
	cloud_->Update();
#pragma endregion

	/// ===AnimaitonModelの更新=== ///
#pragma region Animationモデルの更新
	animationModel_->SetTranslate(modelTranslate_);
	animationModel_->SetRotate(modelRotate_);
	animationModel_->SetColor(modelColor_);
	animationModel_->SetLightData(light_);
	animationModel_->SetCamera(cameraManager_->GetActiveCamera().get());
	animationModel_->Update();
#pragma endregion

	/// ===Particle=== ///
#pragma region Particle
	Particle::Update();
	Particle::SetCamera("Wind", cameraManager_->GetActiveCamera().get());
	Particle::SetCamera("Explosion", cameraManager_->GetActiveCamera().get());
	Particle::SetCamera("Confetti", cameraManager_->GetActiveCamera().get());
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

	/// ===Sprite=== ///
	if (isDisplay_.Sprite) {
		sprite_->Draw(GroundType::Back); // GroundTypeで背景か前景を選択か初期は前景描画。 BlendMode変更可　sprite->Draw(BlendMode::kBlendModeAdd);  
	}

#pragma endregion

#pragma region モデル描画
	// 二重天球
	//sky_->Draw();
	//cloud_->Draw();

	// アニーメーションモデル
	animationModel_->Draw();

	/// ===Model=== ///
	if (isDisplay_.Model) {
		modelLight_->Draw();
		model_->Draw(); // BlendMode変更可能 model_->Draw(BlendMode::kBlendModeAdd);
		model2_->Draw();
		
	}
	/// ===Particle=== ///
	Particle::Draw();

#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}