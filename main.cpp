#include <Windows.h>
#include "System.h"

const wchar_t kWindowTitle[] = L"Engine";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// Systemの初期化
	System::Initialize(kWindowTitle, 1280, 720);

	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	// ウィンドウのxボタンが押されるまでループ
	while (System::ProcessMessage() == 0) {

		// フレームの開始
		System::BeginFrame();

		/* ////////////////////////////
				　　ゲームの処理
		*/ ////////////////////////////

		//System::DrawTriangle();

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();

	return 0;
}