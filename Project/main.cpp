/// ===include=== ///
// Engine
#include "Engine/Core/System.h"
// Game
#include "Game/2d/Sprite.h"
#include "Game/3d/Model.h"
#include "Game/3d/Camera.h"
#include "Game/Audio/Audio.h"
#include "Game/Manager/CameraManager.h"
// Math
#include"Math/sMath.h"
// c++
#include <memory>
#include <string>
// ImGui
#ifdef _DEBUG
#include <imGui.h>
#endif // _DEBUG

const wchar_t kWindowTitle[] = L"Engine";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region 初期化
	// Systemの初期化
	System::Initialize(kWindowTitle, 1280, 720);
	
	// CameraManagerのせいせい
	//NOTE:後々しーんManager等に移動
	std::unique_ptr<CameraManager> cameraManager_;
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize();

	std::unique_ptr<Audio> audio_;
	audio_ = std::make_unique<Audio>();
	audio_->Initialze();
	SoundData soundData = audio_->LoadWave("./Resource/BGM/fanfare.wav");
	SoundData soundData2 = audio_->LoadMP3("./Resource/BGM/clear.mp3");

	//// テクスチャの読み込み
	const std::string& uvTexture = "./Resource/uvChecker.png";
	System::LoadTexture(uvTexture);
	const std::string& monsterBall = "./Resource/monsterBall.png";
	System::LoadTexture(monsterBall);

	// スプライト
	std::unique_ptr<Sprite> sprite;
	sprite = std::make_unique<Sprite>();
	sprite->Initialize();                              // BlendMode変更可　sprite->Initialize(BlendMode::kBlendModeAdd);  
	sprite->SetTexture(uvTexture);                     // テクスチャの設定(これがないと描画できない)
	/* // テクスチャの使い方
	sprite->SetPosition(Vector2(0.0f, 0.0f));          // 場所の設定(初期値は0,0)
	sprite->SetRotation(0.0f);                         // 回転の設定(初期値は0.0);
	sprite->SetSize(Vector2(100.0f, 100.f));           // サイズの設定(初期値は640, 360)
	sprite->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)); // カラーの設定(初期値は1,1,1,1)
	sprite->SetAnchorPoint(Vector2(0.0f, 0.0f));       // アンカーポイントの設定(初期値は0,0)
	sprite->SetTextureSize(Vector2(64.0f, 64.0f));     // テクスチャサイズの設定(初期値は100.0f, 100.0f)
	*/

	// モデルの読み込み
	const std::string& planeModel = "plane";
	System::LoadModel(planeModel);
	const std::string& axisModel = "axis";
	System::LoadModel(axisModel);

	//Camera
	std::shared_ptr<Camera> camera;
	camera = std::make_shared<Camera>();
	camera->Initialize();
	camera->SetTranslate({ 0.0f, 0.0f, -10.0f });
	camera->SetRotate({ 0.0f, 0.0f, 0.0f });

	std::shared_ptr<Camera> camera2;
	camera2 = std::make_shared<Camera>();
	camera2->Initialize();
	camera2->SetTranslate({ 0.0f, 0.0f, -50.0f });
	camera2->SetRotate({ 0.0f, 0.0f, 0.0f });

	cameraManager_->Add("Main", camera);
	cameraManager_->Add("Main2", camera2);

	// モデル
	std::unique_ptr<Model> model;
	model = std::make_unique<Model>();
	model->Initialize(axisModel);

	/* // モデルの使い方
	model->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	model->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	model->SetScale(Vector3(0.0f, 0.0f, 0.0f));
	model->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	model->SetCamera(cameraManager_->GetActiveCamera().get());
	*/

	Vector3 rotate = { 0.0f, 0.0f, 0.0f };       // model
	bool isRotate = false;                       // model
	Vector2 size = { 100.0f, 100.0f };           // sprite
	Vector3 cameraPos = { 0.0f, 0.0f, -10.0f };  // Camera
	Vector3 cameraRotate = { 0.0f, 0.0f, 0.0f }; // Canera
	bool SetCamera = false;

	audio_->SoundPlayWave(soundData);

#pragma endregion

	// ウィンドウのxボタンが押されるまでループ
	while (System::ProcessMessage() == 0) {
		/* ////////////////////////////
				　　更新の処理
		*/ ////////////////////////////
		System::Update();

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
#endif // _DEBUG

		if (SetCamera) {
			cameraManager_->SetActiveCamera("Main");
			audio_->SoundStopWave();
		} else {
			cameraManager_->SetActiveCamera("Main2");
		}

		if (isRotate) {
			rotate.y += 0.1f;
			rotate.x += 0.1f;
			rotate.z -= 0.1f;
		}

		model->SetRotate(rotate);
		model->SetCamera(cameraManager_->GetActiveCamera().get());

		//sprite->SetSize(size);

		camera->SetRotate(cameraRotate);
		camera->SetTranslate(cameraPos);

		cameraManager_->UpdateAllCameras();

		/* ////////////////////////////
				　　描画の処理
		*/ ////////////////////////////
		// フレームの開始
		System::BeginFrame();

		// Spriteの描画
		sprite->Draw();

		// Modelの描画
		model->Draw();

		// フレームの終了
		System::EndFrame();
	}

	audio_->SoundUnload(&soundData);
	System::Finalize();

	return 0;
}