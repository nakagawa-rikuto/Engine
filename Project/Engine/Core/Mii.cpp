#include "Mii.h"

///=====================================================/// 
/// システム全体の初期化
///=====================================================///
void Mii::Initialize(const wchar_t* title, int width, int height) {

	// ゲームウィンドウの作成
	winApp_ = std::make_unique<WinApp>();
	winApp_->CreateGameWindow(title, width, height);

	// DirectXの生成
	dXCommon_ = std::make_unique<DXCommon>();
	dXCommon_->Initialize(winApp_.get(), width, height);

	// InputCommonの生成
	inputCommon_ = std::make_unique<InputCommon>();
	inputCommon_->Initialize(winApp_.get());

	// Keyboardの生成
	keyboard_ = std::make_unique<Keyboard>();
	keyboard_->Initialize(winApp_.get(), inputCommon_->GetDirectInput().Get());

	// Mouseの生成
	mouse_ = std::make_unique<Mouse>();
	mouse_->Initialize(winApp_.get(), inputCommon_->GetDirectInput().Get());

	// Controllerの生成
	controller_ = std::make_unique<Controller>();
	controller_->Initialize();

	// SRVManagerの生成
	srvManager_ = std::make_unique<SRVManager>();
	srvManager_->Initialize(dXCommon_.get());

	// PipelineManagerの生成
	pipelineManager_ = std::make_unique<PipelineManager>();
	pipelineManager_->Initialize(dXCommon_.get());;

	// TextureManagerの生成
	textureManager_ = std::make_unique<TextureManager>();
	textureManager_->Initialize(dXCommon_.get(), srvManager_.get());

	// ModelManagerの生成	
	modelManager_ = std::make_unique<ModelManager>();
	modelManager_->Initialize(textureManager_.get());

	// ImGuiManagerの生成
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApp_.get(), dXCommon_.get(), srvManager_.get());

	// Audiomanagerの生成
	audioManager_ = std::make_unique<AudioManager>();
	audioManager_->Initialze();

	// CSVManagerの生成
	csvManager_ = std::make_unique<CSVManager>();

	// AnimationManagerの生成
	animationManager_ = std::make_unique<AnimationManager>();
}

///=====================================================/// 
/// 更新
///=====================================================///
void Mii::Update() {
	// ImGui
	imGuiManager_->Begin();
	// Input
	keyboard_->Update();
	mouse_->Update();
	controller_->Update();
}

///=====================================================/// 
/// システム全体の終了
///=====================================================///
void Mii::Finalize() {
	// 読み込んだ音声データの一括停止・解放
	audioManager_->StopAll();
	audioManager_->UnloadAll();
	// ImGuiの終了処理
	imGuiManager_->Finalize();
	// ゲームウィンドウの破棄
	winApp_->TerminateGameWindow();

	// 手動の解放
	keyboard_.reset(); // Keyboard
	mouse_.reset(); // Mouse
	controller_.reset(); // Controller
	inputCommon_.reset(); // InputCommon
	modelManager_.reset(); // Modelmanager
	audioManager_.reset(); // AudioManager
	csvManager_.reset(); // CSVManager
	animationManager_.reset(); // AnimationManager
	textureManager_.reset(); // TextrureManager
	pipelineManager_.reset(); // PipelineManager
	imGuiManager_.reset(); // ImGuiManager
	srvManager_.reset(); // SRVManager
	winApp_.reset(); // WinApp
	dXCommon_.reset(); // DXCommon

	// COMの終了
	CoUninitialize();
}


///=====================================================/// 
/// フレーム開始処理
///=====================================================///
void Mii::BeginFrame() {
	dXCommon_->PreDraw();
	imGuiManager_->Draw();
}


///=====================================================/// 
/// フレーム終了処理
///=====================================================///
void Mii::EndFrame() {
	imGuiManager_->End();
	dXCommon_->PostDraw();
}


///=====================================================/// 
/// Windowsのメッセージを処理する
///=====================================================///
int Mii::ProcessMessage() { return winApp_->ProcessMessage(); }


///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
#pragma region 開発者用
// DXCommon
DXCommon* Mii::GetDXCommon() { return dXCommon_.get(); }
// WinApp
WinApp* Mii::GetWinApp() { return winApp_.get(); }
// Keyboard
Keyboard* Mii::GetKeyboard() { return keyboard_.get(); }
// Mouse
Mouse* Mii::GetMouse() { return mouse_.get(); }
// Controller
Controller* Mii::GetController() { return controller_.get(); }
// SRVManager
SRVManager* Mii::GetSRVManager() {return srvManager_.get();}
// PiplelineManager
PipelineManager* Mii::GetPipelineManager() { return pipelineManager_.get(); }
// TextureManager
TextureManager* Mii::GetTextureManager() { return textureManager_.get(); }
// ModelManager
ModelManager* Mii::GetModelManager() { return modelManager_.get(); }
// AudioManager
AudioManager* Mii::GetAudioManager() { return audioManager_.get(); }
// CSVManager
CSVManager* Mii::GetCSVManager() { return csvManager_.get(); }
// AnimationManager
AnimationManager* Mii::GetAnimationManager() { return animationManager_.get(); }
#pragma endregion