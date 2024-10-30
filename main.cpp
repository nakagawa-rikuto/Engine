/// ===include=== ///
// Engine
#include "Base/System/System.h"

// Sprite
#include "Base/Sprite/Sprite.h"
#include "Base/TextureManager/TextureManager.h"

// Math
#include"Base/Math/sMath.h"

// c++
#include <memory>
#include <string>

const wchar_t kWindowTitle[] = L"Engine";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region 初期化
	// Systemの初期化
	System::Initialize(kWindowTitle, 1280, 720);

	//// テクスチャの読み込み
	//const std::string& uvTexture = "./Resource/uvChecker.png";
	//System::LoadTexture(uvTexture);

	const std::string& monsterBall = "./Resource/monsterBall.png";
	System::LoadTexture(monsterBall);

	std::vector<std::unique_ptr<Sprite>> sprites_;
	for (uint32_t i = 0; i < 1; ++i) {
		std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
		//sprite->Initialize(monsterBall);
		sprite->Initialize(monsterBall);
		sprite->SetSize(Vector2(100.0f, 100.0f));
		sprite->SetPosition(Vector2(i * 150.0f, 0.0f));
		sprites_.push_back(std::move(sprite));
	}

#pragma endregion

	// ウィンドウのxボタンが押されるまでループ
	while (System::ProcessMessage() == 0) {

		/* ////////////////////////////
				　　更新の処理
		*/ ////////////////////////////
		System::Update();

		for (auto& sprite : sprites_) {

			sprite->Update();
		}

		// フレームの開始
		System::BeginFrame();

		/* ////////////////////////////
				　　描画の処理
		*/ ////////////////////////////

		// Spriteの描画
		/*for (auto& sprite : sprites_) {

			sprite->Draw();
		}*/

		sprites_.front()->Draw();

		// AL3のmasterを参考にする

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();

	return 0;
}