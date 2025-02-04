#include "Framework.h"
// Mii
#include "Engine/Core/Mii.h"
// Service
#include "Engine/Service/Loader.h"
#include "Engine/Service/Input.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Framework::Initialize(const wchar_t* title) {
	// Miiの初期化
	Mii::Initialize(title, 1280, 720);
	// Loaderの初期化
	Loader::Inititalze(Mii::GetTextureManager(), Mii::GetModelManager(), Mii::GetAudioManager(), Mii::GetCSVManager());
	// Inputの初期化
	Input::Initialize(Mii::GetKeyboard(), Mii::GetMouse(), Mii::GetController());
}

///-------------------------------------------/// 
/// 終了
///-------------------------------------------///
void Framework::Finalize() {
	// 音声データの一括開放
	Loader::AllUnloadSound();
	// 終了処理
	Input::Finalize();
	Loader::Finalize();
	Mii::Finalize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Framework::Update() {
	// システムの更新処理
	Input::Update();
	Mii::Update();
}

///-------------------------------------------/// 
/// 終了チェック
///-------------------------------------------///
bool Framework::IsEndRequst() {return endRequst_;}

///-------------------------------------------/// 
/// 実行
///-------------------------------------------///
void Framework::Run(const wchar_t* title) {
	/// ===ゲームの初期化=== ///
	Initialize(title);
	// ウィンドウのxボタンが押されるまでループ
	while (Mii::ProcessMessage() == 0) {
		/// ===毎フレーム更新=== ///
		Update();
		/// ===終了リクエストが来たら抜ける=== ///
		if (IsEndRequst()) { break; }
		/// ===描画=== ///
		Draw();
	}
	/// ===ゲーム終了=== ///
	Finalize();
}

///-------------------------------------------/// 
/// 描画前処理
///-------------------------------------------///
void Framework::PreDraw() {
	// フレームの開始
	Mii::BeginFrame();
}

///-------------------------------------------/// 
/// 描画後処理
///-------------------------------------------///
void Framework::PostDraw() {
	// フレームの終了
	Mii::EndFrame();
}
