#include "Framework.h"

// Service
#include "Engine/System/Service/ServiceLocator.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Framework::Initialize(const wchar_t* title) {
	/// ===初期化=== ///
	// MiiEnigne
	MiiEngine_ = std::make_unique<Mii>();
	MiiEngine_->Initialize(title, 1280, 720);
	// ServiceLocator
	ServiceLocator::ProvideAll({ 
		MiiEngine_->GetWinApp(),
		MiiEngine_->GetDXCommon(),
		MiiEngine_->GetSRVManager(),
		MiiEngine_->GetRTVManager(),
		MiiEngine_->GetDSVManager(),
		MiiEngine_->GetPipelineManager(),
		MiiEngine_->GetTextureManager(),
		MiiEngine_->GetModelManager(),
		MiiEngine_->GetAnimationManager(),
		MiiEngine_->GetAudioManager(),
		MiiEngine_->GetCSVManager(),
		MiiEngine_->GetOffScreenRenderer(),
		MiiEngine_->GetLineObject3D(),
		MiiEngine_->GetKeyboard(),
		MiiEngine_->GetMouse(),
		MiiEngine_->GetController() }
	);
}

///-------------------------------------------/// 
/// 終了
///-------------------------------------------///
void Framework::Finalize() {
	/// ===終了処理=== ///
	// サービスロケータ
	ServiceLocator::Finalize();
	// MiiEngine
	MiiEngine_->Finalize();
	MiiEngine_.reset();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Framework::Update() {
	/// ===システムの更新処理=== ///
	// MiiEngine
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
