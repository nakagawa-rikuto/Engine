#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/CData.h"
#include "Engine/Input/Input.h"
// Game 
#include "Engine/Graphics/Pipeline/PipelineStateObjectType.h"
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
class SRVManager;
class PipelineManager;
class TextureManager;
class ModelManager;
class ImGuiManager;
class AudioManager;

///=====================================================///
/// システム
///=====================================================///
class Mii {
public:

	Mii() = default;
	~Mii() = default;

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

public:/// ===開発者用関数(その他)=== ///
	///-------------------------------------------/// 
	/// Pipeline
	///-------------------------------------------///
	// PSOをセット
	static void SetPSO(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode);

	///-------------------------------------------/// 
	/// テクスチャ関連
	///-------------------------------------------///
	// SRVインデックス開始番号の取得
	static void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT RootParameterIndex, std::string filePath);
	// GPUハンドルの取得
	static D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(const std::string& filePath);
	// メタデータの取得
	static const DirectX::TexMetadata& GetMetaData(const std::string& filePath);

	///-------------------------------------------/// 
	/// モデル関連
	///-------------------------------------------///
	static ModelData GetModelData(const std::string& filename);

public:/// ===プログラマー用関数=== ///

	///-------------------------------------------/// 
	/// キー入力
	///-------------------------------------------///
	static bool PushKey(BYTE keyNum);
	static bool TriggerKey(BYTE keyNum);

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

	///-------------------------------------------/// 
	/// 音関連
	///-------------------------------------------///
	// 音声データの読み込み
	static void LoadSound(const std::string& key, const std::string& filename, bool loadMP3);
	// 音声データの解放
	static void UnloadSound(const std::string& key);
	// サウンドの再生
	static void PlayeSound(const std::string& key, bool loop);
	// サウンドの停止
	static void StopSound(const std::string& key);
	// 音量の設定
	static void VolumeSound(const std::string& key, float volume);
	// 再生速度の設定
	static void PitchSound(const std::string& key, float pitch);

public:/// ===開発者用関数(Getter)=== ///
	// DXCommonの取得
	static DXCommon* GetDXCommon();
	// deviceの取得
	static ID3D12Device* GetDXDevice();
	// CommandListの取得
	static ID3D12GraphicsCommandList* GetDXCommandList();
	// Inputの取得
	static Input* GetInput();

private:/// ===Variables(変数)=== ///
	static std::unique_ptr<WinApp> winApp_;           // WinApp
	static std::unique_ptr<DXCommon> dXCommon_;       // DirectXCommon
	static std::unique_ptr<Input> input_;             // Input

	static std::unique_ptr<SRVManager> srvManager_;           // SRVManager
	static std::unique_ptr<PipelineManager> pipelineManager_; // PipelineManager
	static std::unique_ptr<TextureManager> textureManager_;   // TextureManager
	static std::unique_ptr<ModelManager> modelManager_;       // ModelManager
	static std::unique_ptr<ImGuiManager> imGuiManager_;       // ImGuiManager
	static std::unique_ptr<AudioManager> audioManager_;       // AudioMangaer
};