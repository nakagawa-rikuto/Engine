#include "System.h"
#include "GameScene.h"
#include "Input.h"
#include"MyMath.h"

const wchar_t kWindowTitle[] = L"Engine";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	Input* input = nullptr; // Inputの宣言
	GameScene* gameScene = nullptr; // GameSceneの宣言

#pragma region 初期化
	// Systemの初期化
	System::Initialize(kWindowTitle, 1280, 720);

	// 入力の初期化
	input = Input::GetInstance();

	// GameSceneの初期化
	gameScene = new GameScene();
	gameScene->Initialize();
#pragma endregion

	// ウィンドウのxボタンが押されるまでループ
	while (System::ProcessMessage() == 0) {

		///* ///////////////////////////////////////////////
		//					三角形の情報
		//*/ ///////////////////////////////////////////////

		//TransformInfo transformTriangle;
		//transformTriangle = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };

		/* ////////////////////////////
				　　更新の処理
		*/ ////////////////////////////
		System::Update();
		gameScene->Update();

		// フレームの開始
		System::BeginFrame();

		/* ////////////////////////////
				　　描画の処理
		*/ ////////////////////////////
		gameScene->Draw();

		// AL3のmasterを参考にする

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();

	return 0;
}