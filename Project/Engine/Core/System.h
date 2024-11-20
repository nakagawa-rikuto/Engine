#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/CData.h"
// DirectXTex
#include "DirectXTex.h"
// C++
#include <string>
#include <d3d12.h>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <span>
#include <dxgidebug.h>
#include <wrl.h>
#include <memory>

/// ===前方宣言=== ///
// Engine
class WinApp;
class DXCommon;
class Input;
// Manager
class TextureManager;
class SRVManager;
class ModelManager;

///=====================================================///
/// システム
///=====================================================///
class System {
public:

	System() = default;
	~System() = default;

public:/// ===開発者用関数(システム)=== ///
	
	// システム全体の初期化
	static void Initialize(const wchar_t* title, int width = 1280, int height = 720);
	// システム全体の更新
	static void Update();
	// システム全体の終了
	static void Finalize();
	// フレーム開始処理
	static void BeginFrame();
	// フレーム終了処理
	static void EndFrame();
	// Windowsのメッセージを処理
	static int ProcessMessage();
	// ImGuiの開始処理
	static void BeginImGui();
	// ImGuiの終了処理
	static void EndImGui();

public:/// ===開発者用関数(その他)=== ///

	///-------------------------------------------/// 
	/// テクスチャ関連
	///-------------------------------------------///
	// SRVインデックス開始番号の取得
	static uint32_t GetTextureIndexByFilePath(const std::string& filePath);
	// GPUハンドルの取得
	static D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(uint32_t textureIndex);
	// メタデータの取得
	static const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);

	///-------------------------------------------/// 
	/// モデル関連
	///-------------------------------------------///
	static ModelData GetModelData(const std::string& filename);

public:/// ===プログラマー用関数=== ///

	///-------------------------------------------/// 
	/// テクスチャ関連
	///-------------------------------------------///
	// テクスチャの読み込み
	static void LoadTexture(const std::string& filePath);

	///-------------------------------------------/// 
	/// モデル関連
	///-------------------------------------------///
	// モデルの読み込み
	static void LoadModel(const std::string& filename);

public:/// ===開発者用関数(Getter)=== ///
	// DXCommonの取得
	static DXCommon* GetDXCommon();
	// deviceの取得
	static ID3D12Device* GetDXDevice();
	// CommandListの取得
	static ID3D12GraphicsCommandList* GetDXCommandList();

private:/// ===Variables(変数)=== ///

	static std::unique_ptr<WinApp> winApp_;           // WinApp
	static std::unique_ptr<DXCommon> dXCommon_;       // DirectXCommon
	static std::unique_ptr<Input> input_;             // Input

	static std::unique_ptr<TextureManager> textureManager_; // TextureManager
	static std::unique_ptr<SRVManager> srvManager_;         // SRVManager
	static std::unique_ptr<ModelManager> modelManager_;     // ModelManager
};