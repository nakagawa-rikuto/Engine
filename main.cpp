/// ===include=== ///
// Engine
#include "Base/System/System.h"

// Sprite
#include "Base/Sprite/Sprite.h"
#include "Base/TextureManager/TextureManager.h"
#include "Base/PSO/PipelineStateObjectType.h"

// Model
#include "Base/Model/Model.h"

// Math
#include"Base/Math/sMath.h"

// c++
#include <memory>
#include <string>

#ifdef _DEBUG
#include <imGui.h>
#endif // _DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	sprite->Initialize();                              // BlendMode変更可　sprite->Initialize(BlendMode::kBlendModeAdd);  
	sprite->SetTexture(uvTexture);                     // テクスチャの設定(これがないと描画できない)
	/* // テクスチャの使い方
	sprite->SetPosition(Vector2(0.0f, 0.0f));          // 場所の設定(初期値は0,0)
	sprite->SetRotation(0.0f);                         // 回転の設定(初期値は0.0);
	sprite->SetSize(Vector2(100.0f, 100.f));           // サイズの設定(初期値は640, 360)
	sprite->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f)); // カラーの設定(初期値は1,1,1,1)
	sprite->SetAnchorPoint(Vector2(0.0f, 0.0f));       // アンカーポイントの設定(初期値は0,0)
	sprite->SetTextureSize(Vector2(64.0f, 64.0f));     // テクスチャサイズの設定(初期値は100.0f, 100.0f)
	*/

	// モデルの読み込み
	const std::string& planeModel = "plane";
	System::LoadModel(planeModel);
	const std::string& axisModel = "axis";
	System::LoadModel(axisModel);

	// モデル
	std::unique_ptr<Model> model;
	model = std::make_unique<Model>();
	model->Initialize(axisModel);
	/* // モデルの使い方
	model->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	model->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
	model->SetScale(Vector3(0.0f, 0.0f, 0.0f));
	model->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	*/

	Vector3 rotate = { 0.0f, 0.0f, 0.0f };

#pragma endregion

	// ウィンドウのxボタンが押されるまでループ
	while (System::ProcessMessage() == 0) {
		System::BeginImGui();
		/* ////////////////////////////
				　　更新の処理
		*/ ////////////////////////////
		System::Update();

#ifdef _DEBUG
		ImGui::Begin("model");
		ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
		ImGui::End();
#endif // _DEBUG
		
		rotate.y += 0.1f;

		model->SetRotate(rotate);

		/* ////////////////////////////
				　　描画の処理
		*/ ////////////////////////////
		// フレームの開始
		System::BeginFrame();

		// Spriteの描画
		sprite->Draw();

		// Modelの描画
		model->Draw();

		// フレームの終了
		System::EndFrame();
	}

	System::EndImGui();
	System::Finalize();

	return 0;
}