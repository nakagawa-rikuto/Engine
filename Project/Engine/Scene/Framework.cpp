#include "Framework.h"

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
	// Miiの生成
	MiiEngine_ = std::make_unique<Mii>();
	MiiEngine_->Initialize(title, 1280, 720);
	// Getterの初期化
	Getter::Initialize(MiiEngine_->GetDXCommon(), MiiEngine_->GetWinApp(), MiiEngine_->GetSRVManager(), MiiEngine_->GetModelManager(), MiiEngine_->GetTextureManager(), MiiEngine_->GetAnimationManager());
	// Loaderの初期化
	Loader::Inititalze(MiiEngine_->GetTextureManager(), MiiEngine_->GetModelManager(), MiiEngine_->GetAudioManager(), MiiEngine_->GetCSVManager(), MiiEngine_->GetAnimationManager());
	// Rendererの初期化
	Render::Initialize(MiiEngine_->GetPipelineManager(), MiiEngine_->GetTextureManager());
	// Inputの初期化
	Input::Initialize(MiiEngine_->GetKeyboard(), MiiEngine_->GetMouse(), MiiEngine_->GetController());
	// Audioの初期化
	Audio::Initialze(MiiEngine_->GetAudioManager());
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
	MiiEngine_->Finalize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Framework::Update() {
	// システムの更新処理
	Input::Update();
	MiiEngine_->Update();
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
	while (MiiEngine_->ProcessMessage() == 0) {
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
	MiiEngine_->BeginFrame();
}

///-------------------------------------------/// 
/// 描画後処理
///-------------------------------------------///
void Framework::PostDraw() {
	// フレームの終了
	MiiEngine_->EndFrame();
}
