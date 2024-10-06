#include "System.h"

#include "Sprite.h"

#include"sMath.h"

const wchar_t kWindowTitle[] = L"Engine";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region 初期化
	// Systemの初期化
	System::Initialize(kWindowTitle, 1280, 720);

	std::vector<Sprite*> sprites_;
	for (uint32_t i = 0; i < 5; ++i) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(System::GetSpriteCommon());
		sprite->SetSize(Vector2(50.0f, 50.0f));
		sprite->SetPosition(Vector2(i * 150.0f, 0.0f));
		sprites_.push_back(sprite);
	}

#pragma endregion

	// ウィンドウのxボタンが押されるまでループ
	while (System::ProcessMessage() == 0) {

		/* ////////////////////////////
				　　更新の処理
		*/ ////////////////////////////
		System::Update();
		
		for (Sprite* sprite : sprites_) {

			sprite->Update();
		}

		// フレームの開始
		System::BeginFrame();

		/* ////////////////////////////
				　　描画の処理
		*/ ////////////////////////////

		// Spriteの描画
		for (Sprite* sprite : sprites_) {

			sprite->Draw();
		}

		// AL3のmasterを参考にする

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();

	return 0;
}