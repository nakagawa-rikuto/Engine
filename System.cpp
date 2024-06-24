#include "System.h"
#include "WinApp.h"

#include <d3d12.h>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <span>
#include <dxgidebug.h>
#include <wrl.h>
WinApp* System::winApp = nullptr;
DXCommon* System::dXCommon = nullptr;

/// <summary>
/// ReportLiveObjects
/// </summary>
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


/// <summary>
/// システム全体の初期化
/// </summary>
void System::Initialize(const wchar_t* title, int width, int height) {

	// ReportLiveObjects
	D3DResourceLeakChecker leakCheck;

	// ゲームウィンドウの作成
	winApp = WinApp::GetInstance();
	winApp->CreateGameWindow(title, width, height);

	// DirectX初期化処理
	dXCommon = DXCommon::GetInstance();
	dXCommon->Initialize(winApp, width, height);
}

/// <summary>
/// システム全体の終了
/// </summary>
void System::Finalize() {

	// ゲームウィンドウの破棄
	winApp->TerminateGameWindow();
}

/// <summary>
/// フレーム開始処理
/// </summary>
void System::BeginFrame() {
	dXCommon->PreDraw();
}

/// <summary>
/// フレーム終了処理
/// </summary>
void System::EndFrame() {
	dXCommon->PostDraw();
}

/// <summary>
/// Windowsのメッセージを処理する
/// </summary>
int System::ProcessMessage() { return winApp->ProcessMessage(); }
