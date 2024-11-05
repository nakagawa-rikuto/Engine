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
	const std::string& uvTexture = "./Resource/uvChecker.png";
	System::LoadTexture(uvTexture);
	const std::string& monsterBall = "./Resource/monsterBall.png";
	System::LoadTexture(monsterBall);

	// スプライト
	std::unique_ptr<Sprite> sprite;
	sprite = std::make_unique<Sprite>();
	sprite->Initialize(uvTexture);
	sprite->SetPosition(Vector2(640.0f, 360.0f));      // Vector2
	sprite->SetRotation(0.0f);                         // float
	sprite->SetSize(Vector2(500.0f, 500.f));           // Vector2	
	//sprite->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f)); // Vector4
	sprite->SetAnchorPoint(Vector2(0.5f, 0.5f));       // Vector2
	sprite->SetTextureSize(Vector2(64.0f, 64.0f));     // Vector2

	float rotation = 0.0f;

#pragma endregion

	// ウィンドウのxボタンが押されるまでループ
	while (System::ProcessMessage() == 0) {

		/* ////////////////////////////
				　　更新の処理
		*/ ////////////////////////////
		System::Update();

		//rotation += 0.1f;

		sprite->SetRotation(rotation);

		// Spriteの更新
		sprite->Update();

		// フレームの開始
		System::BeginFrame();

		/* ////////////////////////////
				　　描画の処理
		*/ ////////////////////////////

		// Spriteの描画
		sprite->Draw();

		// フレームの終了
		System::EndFrame();
	}

	System::Finalize();

	return 0;
}