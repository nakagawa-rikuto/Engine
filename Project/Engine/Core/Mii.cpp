#include "Mii.h"
// Engine
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
// Input
#include "Engine/Input/Keyboard.h"
#include "Engine/Input/Mouse.h"
#include "Engine/Input/Controller.h"
// Manager
#include "Engine/Managers/SRVManager.h"
#include "Engine/Managers/PiplineManager.h"
#include "Engine/Managers/TextureManager.h"
#include "Engine/Managers/ModelManager.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Engine/Managers/AudioManager.h"
#include "Engine/Managers/CSVManager.h"
// Math
#include "Math/sMath.h"

/// ===宣言=== ///
// Engine
std::unique_ptr<WinApp> Mii::winApp_ = nullptr;
std::unique_ptr<DXCommon> Mii::dXCommon_ = nullptr;
// Input
std::unique_ptr<InputCommon> Mii::inputCommon_ = nullptr;
std::unique_ptr<Keyboard> Mii::keyboard_ = nullptr;
std::unique_ptr<Mouse> Mii::mouse_ = nullptr;
std::unique_ptr<Controller> Mii::controller_ = nullptr;
// Manager
std::unique_ptr<SRVManager> Mii::srvManager_ = nullptr;
std::unique_ptr<PipelineManager> Mii::pipelineManager_ = nullptr;
std::unique_ptr<TextureManager> Mii::textureManager_ = nullptr;
std::unique_ptr<ModelManager> Mii::modelManager_ = nullptr;
std::unique_ptr<ImGuiManager> Mii::imGuiManager_ = nullptr;
std::unique_ptr<AudioManager> Mii::audioManager_ = nullptr;
std::unique_ptr<CSVManager> Mii::csvManager_ = nullptr;

///=====================================================/// 
/// ReportLiveObjects()
///=====================================================///
struct D3DResourceLeakChecker {

	~D3DResourceLeakChecker() {

		// リソースリークチェック
		ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

///=====================================================/// 
/// システム全体の初期化
///=====================================================///
void Mii::Initialize(const wchar_t* title, int width, int height) {

	// ReportLiveObjects
	static D3DResourceLeakChecker leakCheck;

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
	controller_->Initialize(winApp_.get(), inputCommon_->GetDirectInput().Get());

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
	modelManager_->Initialize(dXCommon_.get());

	// ImGuiManagerの生成
	imGuiManager_ = std::make_unique<ImGuiManager>();
	imGuiManager_->Initialize(winApp_.get(), dXCommon_.get(), srvManager_.get());

	// Audiomanagerの生成
	audioManager_ = std::make_unique<AudioManager>();
	audioManager_->Initialze();

	// CSVManagerの生成
	csvManager_ = std::make_unique<CSVManager>();
}

///=====================================================/// 
/// 更新
///=====================================================///
void Mii::Update() {
	imGuiManager_->Begin();
}

///=====================================================/// 
/// システム全体の終了
///=====================================================///
void Mii::Finalize() {
	// 読み込んだ音声データの一括解放
	audioManager_->UnloadAll();
	// ImGuiの終了処理
	imGuiManager_->Finalize();
	// ゲームウィンドウの破棄
	winApp_->TerminateGameWindow();

	// 手動の解放
	winApp_.reset();
	dXCommon_.reset();
	inputCommon_.reset();
	keyboard_.reset();
	mouse_.reset();
	controller_.reset();
	srvManager_.reset();
	pipelineManager_.reset();
	textureManager_.reset();
	modelManager_.reset();
	imGuiManager_.reset();
	audioManager_.reset();

	// COMの終了
	CoUninitialize();
}


///=====================================================/// 
/// フレーム開始処理
///=====================================================///
void Mii::BeginFrame() {
	dXCommon_->PreDraw();
#ifdef _DEBUG
	imGuiManager_->Draw();
#endif // _DEBUG
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
/// 開発者用関数
///-------------------------------------------///
#pragma region Pipeline関連
// PSOの取得
void Mii::SetPSO(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode) { pipelineManager_->SetPipeline(commandList, type, mode); }
#pragma endregion
#pragma region Texture関連
// SRVインデックス開始番号の取得
void Mii::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT RootParameterIndex, std::string key) { textureManager_->SetGraphicsRootDescriptorTable(commandList, RootParameterIndex, key); }
// メタデータの取得
const DirectX::TexMetadata& Mii::GetMetaData(const std::string& key) { return textureManager_->GetMetaData(key); }
#pragma endregion
#pragma region Model関連
// モデルデータの取得
ModelData Mii::GetModelData(const std::string& filename) { return modelManager_->GetModelData(filename); }
#pragma endregion


///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
#pragma region 開発者用
// DXCommon
DXCommon* Mii::GetDXCommon() { return dXCommon_.get(); }
// device
ID3D12Device* Mii::GetDXDevice() { return dXCommon_->GetDevice(); }
// CommandList
ID3D12GraphicsCommandList* Mii::GetDXCommandList() { return dXCommon_->GetCommandList(); }
// Keyboard
Keyboard* Mii::GetKeyboard() { return keyboard_.get(); }
// Mouse
Mouse* Mii::GetMouse() { return mouse_.get(); }
// Controller
Controller* Mii::GetController() { return controller_.get(); }
// SRVManager
SRVManager* Mii::GetSRVManager() {return srvManager_.get();}
// TextureManager
TextureManager* Mii::GetTextureManager() { return textureManager_.get(); }
// ModelManager
ModelManager* Mii::GetModelManager() { return modelManager_.get(); }
// AudioManager
AudioManager* Mii::GetAudioManager() { return audioManager_.get(); }
// CSVManager
CSVManager* Mii::GetCSVManager() { return csvManager_.get(); }
#pragma endregion