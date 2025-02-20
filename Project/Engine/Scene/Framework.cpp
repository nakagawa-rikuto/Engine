#include "Framework.h"
// Mii
#include "Engine/Core/Mii.h"
// Service
#include "Engine/System/Service/Loader.h"
#include "Engine/System/Service/Input.h"
#include "Engine/System/Service/Audio.h"
#include "Engine/System/Service/Getter.h"
#include "Engine/System/Service/Render.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Framework::Initialize(const wchar_t* title) {
	// Miiの初期化
	Mii::Initialize(title, 1280, 720);
	// Getterの初期化
	Getter::Initialize(Mii::GetDXCommon(), Mii::GetWinApp(), Mii::GetSRVManager(), Mii::GetModelManager(), Mii::GetTextureManager(), Mii::GetAnimationManager());
	// Loaderの初期化
	Loader::Inititalze(Mii::GetTextureManager(), Mii::GetModelManager(), Mii::GetAudioManager(), Mii::GetCSVManager(), Mii::GetAnimationManager());
	// Rendererの初期化
	Render::Initialize(Mii::GetPipelineManager(), Mii::GetTextureManager());
	// Inputの初期化
	Input::Initialize(Mii::GetKeyboard(), Mii::GetMouse(), Mii::GetController());
	// Audioの初期化
	Audio::Initialze(Mii::GetAudioManager());
	
}

///-------------------------------------------/// 
/// 終了
///-------------------------------------------///
void Framework::Finalize() {
	// 終了処理
	Audio::Finalize();
	Input::Finalize();
	Render::Finalize();
	Loader::Finalize();
	Getter::Finalize();
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
