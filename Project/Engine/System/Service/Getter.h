#pragma once
/// ===Include=== ///
// C++
#include <string>
#include <d3d12.h>
// DirectXTex
#include "DirectXTex.h"
// Engine
#include "Engine/DataInfo/CData.h"
#include "Engine/DataInfo/AnimationData.h"

/// ===前方宣言=== ///
class DXCommon;
class WinApp;
class SRVManager;
class ModelManager;
class TextureManager;
class AnimationManager;

///=====================================================/// 
/// Getterのサービスロケータ
///=====================================================///
class Getter {
public:

	Getter() = default;
	~Getter() = default;

	// 初期化
	static void Initialize(DXCommon* dxCommon, WinApp* winApp, SRVManager* srvManager, ModelManager* modelManager, TextureManager* textureManager, AnimationManager* animationManager);
	// 終了処理
	static void Finalize();
	// Deviceの取得
	static ID3D12Device* GetDXDevice();
	// RTVの指定番号のCPUでスクリプタハンドルを取得する
	static D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);
	// RTVの指定番号のGPUでスクリプタハンドルを取得する
	static D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);
	// DSVの指定番号のCPUでスクリプタハンドルを取得する
	static D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
	// DSVの指定番号のGPUでスクリプタハンドルを取得する
	static D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);
	// SRVの指定番号のCPUでスクリプタハンドルを取得する
	static D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);
	// SRVの指定番号のGPUでスクリプタハンドルを取得する
	static D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);
	// ウィンドウの幅を取得する
	static const int GetWindowWidth();
	// ウィンドウの高さを取得する
	static const int GetWindowHeight();
	// TextureDataの取得
	static const DirectX::TexMetadata& GetMetaData(const std::string& key);
	// ModelDataの取得
	static ModelData GetModelData(const std::string& directorPath);
	// AnimationDataの取得
	static Animation GetAnimationData(const std::string& directorPath);

private:

	static WinApp* winApp_;                     // WinApp
	static DXCommon* dXCommon_;                 // DirectXCommon
	static SRVManager* srvManager_;             // SRVManager
	static TextureManager* textureManager_;     // TextureManager
	static ModelManager* modelManager_;         // ModelManager
	static AnimationManager* animationManager_; // AnimationManager
};