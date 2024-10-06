#include "System.h"

#include "WinApp.h"
#include "DXCommon.h"
#include "Input.h"

#include "SpriteCommon.h"

#include "sMath.h"

std::unique_ptr<WinApp> System::winApp_ = nullptr;
std::unique_ptr<DXCommon> System::dXCommon_ = nullptr;
std::unique_ptr<Input> System::input_ = nullptr;

std::unique_ptr<SpriteCommon> System::spriteCommon_ = nullptr;

///=====================================================/// 
/// ReportLiveObjects()
///=====================================================///
struct D3DResourceLeakChecker {

	~D3DResourceLeakChecker() {

		// リソースリークチェック
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// SpriteCommon
SpriteCommon* System::GetSpriteCommon() { return spriteCommon_.get(); }

///=====================================================/// 
/// システム全体の初期化
///=====================================================///
void System::Initialize(const wchar_t* title, int width, int height) {

	// ReportLiveObjects
	D3DResourceLeakChecker leakCheck;

	// ゲームウィンドウの作成
	winApp_ = std::make_unique<WinApp>();
	winApp_->CreateGameWindow(title, width, height);

	// DirectX初期化処理
	dXCommon_ = std::make_unique<DXCommon>();
	dXCommon_->Initialize(winApp_.get(), width, height);

	// Inputの初期化
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	// スプライト共通部の生成
	spriteCommon_ = std::make_unique<SpriteCommon>();
	spriteCommon_->Initialize(dXCommon_.get());
}

///=====================================================/// 
/// 更新
///=====================================================///
void System::Update() {

	input_->Update();
}

///=====================================================/// 
/// システム全体の終了
///=====================================================///
void System::Finalize() {

	// ゲームウィンドウの破棄
	winApp_->TerminateGameWindow();
}

///=====================================================/// 
/// フレーム開始処理
///=====================================================///
void System::BeginFrame() {
	dXCommon_->PreDraw();
	spriteCommon_->PreDraw();
}

///=====================================================/// 
/// フレーム終了処理
///=====================================================///
void System::EndFrame() {
	dXCommon_->PostDraw();
}

///=====================================================/// 
/// Windowsのメッセージを処理する
///=====================================================///
int System::ProcessMessage() { return winApp_->ProcessMessage(); }
