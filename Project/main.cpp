/// ===include=== ///
#include "Engine/Scene/Framework.h"
#include "application/Game/Scene/MyGame.h"

/// ===ゲームタイトル=== ///
const wchar_t kWindowTitle[] = L"2325_神経神経衰弱";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 宣言
	Framework* game = new MyGame();
	// 実行
	game->Run(kWindowTitle);
	// 解放
	delete game;
	return 0;
}