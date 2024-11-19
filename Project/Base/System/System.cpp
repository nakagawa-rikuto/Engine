#include "System.h"

// Engine
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DXCommon.h"
#include "Base/Input/Input.h"

// Sprite
#include "Base/TextureManager/TextureManager.h"
#include "Base/SRVManager/SRVManager.h"

// Model
#include "Base/ModelManager/ModelManager.h"

// Math
#include "Base/Math/sMath.h"

/// ===宣言=== ///
// Engine
std::unique_ptr<WinApp> System::winApp_ = nullptr;
std::unique_ptr<DXCommon> System::dXCommon_ = nullptr;
std::unique_ptr<Input> System::input_ = nullptr;
// Sprite
std::unique_ptr<TextureManager> System::textureManager_ = nullptr;
std::unique_ptr<SRVManager> System::srvManager_ = nullptr;
// Model
std::unique_ptr<ModelManager> System::modelManager_ = nullptr;

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
void System::Initialize(const wchar_t* title, int width, int height) {

	// ReportLiveObjects
	static D3DResourceLeakChecker leakCheck;

	// ゲームウィンドウの作成
	winApp_ = std::make_unique<WinApp>();
	winApp_->CreateGameWindow(title, width, height);

	// DirectXの生成
	dXCommon_ = std::make_unique<DXCommon>();
	dXCommon_->Initialize(winApp_.get(), width, height);

	// Inputの生成
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	// TextrueManagerの生成
	textureManager_ = std::make_unique<TextureManager>();
	textureManager_->Initialize(dXCommon_.get());

	// SRVManagerの生成
	srvManager_ = std::make_unique<SRVManager>();
	srvManager_->Initialize(dXCommon_.get());

	// ModelManagerの生成	
	modelManager_ = std::make_unique<ModelManager>();
	modelManager_->Initialize(dXCommon_.get());
}

///=====================================================/// 
/// 更新
///=====================================================///
void System::Update() {

	input_->Update();
}

///=====================================================/// 
/// システム全体の終了
///=====================================================///
void System::Finalize() {

	// ゲームウィンドウの破棄
	winApp_->TerminateGameWindow();

	// 手動の解放
	winApp_.reset();
	dXCommon_.reset();
	input_.reset();
	textureManager_.reset();
	srvManager_.reset();

	// COMの終了
	CoUninitialize();
}

///=====================================================/// 
/// フレーム開始処理
///=====================================================///
void System::BeginFrame() {
	dXCommon_->PreDraw();
}

///=====================================================/// 
/// フレーム終了処理
///=====================================================///
void System::EndFrame() {
	dXCommon_->PostDraw();
}

///=====================================================/// 
/// Windowsのメッセージを処理する
///=====================================================///
int System::ProcessMessage() { return winApp_->ProcessMessage(); }

///=====================================================/// 
/// ImGuiの開始処理
///=====================================================///
void System::BeginImGui() { return dXCommon_->BeginImGui(); }

///=====================================================/// 
/// ImGuiの終了処理
///=====================================================///
void System::EndImGui() { return dXCommon_->EndImGui(); }

#pragma region Sprite関連
// テクスチャ読み込み
void System::LoadTexture(const std::string& filePath) {textureManager_->LoadTexture(filePath);}
// SRVインデックス開始番号の取得
uint32_t System::GetTextureIndexByFilePath(const std::string& filePath) {return textureManager_->GetTextureIndexByFilePath(filePath);}
// GPUハンドルの取得
D3D12_GPU_DESCRIPTOR_HANDLE System::GetSRVHandleGPU(uint32_t textureIndex) {return textureManager_->GetSRVHandleGPU(textureIndex);}
// メタデータの取得
const DirectX::TexMetadata& System::GetMetaData(uint32_t textureIndex) {return textureManager_->GetMetaData(textureIndex);}
#pragma endregion

#pragma region Model関連
// モデルの読み込み
void System::LoadModel(const std::string& filename) { modelManager_->LoadModel("Resource", filename); }
// モデルデータの取得
ModelData System::GetModelData(const std::string& filename) { return modelManager_->GetModelData(filename); }
#pragma endregion

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// DXCommon
DXCommon* System::GetDXCommon() { return dXCommon_.get(); }
// device
ID3D12Device* System::GetDXDevice() { return dXCommon_->GetDevice(); }
// CommandList
ID3D12GraphicsCommandList* System::GetDXCommandList() { return dXCommon_->GetCommandList(); }