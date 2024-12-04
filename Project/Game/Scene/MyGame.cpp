#include "MyGame.h"
// Math
#include"Math/sMath.h"
// c++
#include <memory>
#include <string>
// ImGui
#ifdef _DEBUG
#include <imGui.h>
#endif // _DEBUG

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
MyGame::MyGame() = default;
MyGame::~MyGame() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void MyGame::Initialize(const wchar_t* title) {
	// 基底クラスの初期化
	Framework::Initialize(title);
	// Systemの初期化
	System::Initialize(title, 1280, 720);

	// CameraManagerのせいせい
	//NOTE:後々しーんManager等に移動
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize();

	System::LoadSound("fanfare", "./Resource/BGM/fanfare.wav", false);
	System::LoadSound("clear", "./Resource/BGM/clear.mp3", true);

	//// テクスチャの読み込み
	const std::string& uvTexture = "./Resource/uvChecker.png";
	System::LoadTexture(uvTexture);
	const std::string& monsterBall = "./Resource/monsterBall.png";
	System::LoadTexture(monsterBall);

	// スプライト
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
	model = std::make_unique<Model>();
	model->Initialize(axisModel);

	/* // モデルの使い方
	model->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	model->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	model->SetScale(Vector3(0.0f, 0.0f, 0.0f));
	model->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	model->SetCamera(cameraManager_->GetActiveCamera().get());
	*/

	//System::PlayeSound("clear", false);
}
 
///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void MyGame::Finalize() {
	// Systemの終了処理
	System::Finalize();
	// 基底クラスの終了処理
	Framework::Finalize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void MyGame::Update() {
	// 基底クラスの更新処理
	Framework::Update();
	// システムの更新処理
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
		System::PlayeSound("clear", false);
		System::VolumeSound("clear", volume);
		System::PitchSound("clear", pitch);
	} else {
		System::StopSound("clear");
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
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void MyGame::Draw() {
	// フレームの開始
	System::BeginFrame();

	// Spriteの描画
	sprite->Draw();

	// Modelの描画
	model->Draw();

	// フレームの終了
	System::EndFrame();
}