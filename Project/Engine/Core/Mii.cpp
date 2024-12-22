#include "Mii.h"
// Engine
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
#include "Engine/Input/Input.h"
// Manager
#include "Engine/Managers/SRVManager.h"
#include "Engine/Managers/PiplineManager.h"
#include "Engine/Managers/TextureManager.h"
#include "Engine/Managers/ModelManager.h"
#include "Engine/Managers/ImGuiManager.h"
#include "Engine/Managers/AudioManager.h"
// Math
#include "Math/sMath.h"

/// ===宣言=== ///
// Engine
std::unique_ptr<WinApp> Mii::winApp_ = nullptr;
std::unique_ptr<DXCommon> Mii::dXCommon_ = nullptr;
std::unique_ptr<Input> Mii::input_ = nullptr;
// Manager
std::unique_ptr<SRVManager> Mii::srvManager_ = nullptr;
std::unique_ptr<PipelineManager> Mii::pipelineManager_ = nullptr;
std::unique_ptr<TextureManager> Mii::textureManager_ = nullptr;
std::unique_ptr<ModelManager> Mii::modelManager_ = nullptr;
std::unique_ptr<ImGuiManager> Mii::imGuiManager_ = nullptr;
std::unique_ptr<AudioManager> Mii::audioManager_ = nullptr;

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

	// Inputの生成
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());

	// SRVManagerの生成
	srvManager_ = std::make_unique<SRVManager>();
	srvManager_->Initialize(dXCommon_.get());

	// PipelineManagerの生成
	pipelineManager_ = std::make_unique<PipelineManager>();
	pipelineManager_->Initialize();

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
}

///=====================================================/// 
/// 更新
///=====================================================///
void Mii::Update() {

	input_->Update();
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
	input_.reset();
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
/// 開発者用関数
///-------------------------------------------///
#pragma region Pipeline関連
// PSOの取得
void Mii::SetPSO(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode) { pipelineManager_->SetPipeline(commandList, type, mode); }
#pragma endregion
#pragma region Texture関連
// SRVインデックス開始番号の取得
void Mii::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT RootParameterIndex, std::string filePath) { textureManager_->SetGraphicsRootDescriptorTable(commandList, RootParameterIndex, filePath); }
// GPUハンドルの取得
D3D12_GPU_DESCRIPTOR_HANDLE Mii::GetSRVHandleGPU(const std::string& filePath) { return textureManager_->GetSRVHandleGPU(filePath); }
// メタデータの取得
const DirectX::TexMetadata& Mii::GetMetaData(const std::string& filePath) { return textureManager_->GetMetaData(filePath); }
#pragma endregion


///-------------------------------------------/// 
/// プログラマー用関数
///-------------------------------------------///
#pragma region Texture関連
// テクスチャ読み込み
void Mii::LoadTexture(const std::string& filePath) {textureManager_->LoadTexture(filePath);}
#pragma endregion
#pragma region Model関連
// モデルの読み込み
void Mii::LoadModel(const std::string& filename) { modelManager_->LoadModel("Resource", filename); }
// モデルデータの取得
ModelData Mii::GetModelData(const std::string& filename) { return modelManager_->GetModelData(filename); }
#pragma endregion
#pragma region Audio関連
// 音声データの読み込み
void Mii::LoadSound(const std::string& key, const std::string& filename, bool loadMP3) { return audioManager_->Load(key, filename, loadMP3); }
// 音声データの解放
void Mii::UnloadSound(const std::string& key) { return audioManager_->Unload(key); }
// 音声データの再生
void Mii::PlayeSound(const std::string& key, bool loop) { return audioManager_->Play(key, loop); }
// 音声データの停止
void Mii::StopSound(const std::string& key) { return audioManager_->Stop(key); }
// 音声データの音量設定
void Mii::VolumeSound(const std::string& key, float volume) { return audioManager_->SetVolume(key, volume); }
// 音声データの再生速度設定
void Mii::PitchSound(const std::string& key, float pitch) { return audioManager_->setPitch(key, pitch); }
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
// Input
Input* Mii::GetInput() { return input_.get(); }
#pragma endregion