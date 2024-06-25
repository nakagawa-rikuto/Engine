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

		VertexDataTriangle triangle1;
		triangle1.position = { -0.5f, -0.5f, 0.0f, 1.0f };

		VertexDataTriangle triangle2;
		triangle2.position = { 0.0f, 0.5f, 0.0f, 1.0f };

		VertexDataTriangle triangle3;
		triangle3.position = { 0.5f, -0.5f, 0.0f, 1.0f };

		// フレームの開始
		System::BeginFrame();

		/* ////////////////////////////
				　　ゲームの処理
		*/ ////////////////////////////

		System::DrawTriangle(&triangle1, &triangle2, &triangle3);

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();

	return 0;
}