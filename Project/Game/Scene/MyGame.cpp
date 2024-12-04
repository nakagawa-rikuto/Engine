#include "MyGame.h"
// System
#include "Engine/Core/System.h"
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

	// シーンの初期化
	/*scene_ = std::make_unique<GameScene>();
	scene_->Initialize();*/

	// シーンマネージャの初期化
	sceneManager_ = std::make_unique<SceneManager>();
	sceneManager_->ChangeScene(SceneManager::kGame);
}
 
///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void MyGame::Finalize() {
	// シーンの解放処理
	scene_.reset();
	sceneManager_.reset();
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
	// シーンの更新
	//scene_->Update();
	sceneManager_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void MyGame::Draw() {
	// フレームの開始
	System::BeginFrame();
	// シーンの描画
	//scene_->Draw();
	sceneManager_->Draw();
	// フレームの終了
	System::EndFrame();
}