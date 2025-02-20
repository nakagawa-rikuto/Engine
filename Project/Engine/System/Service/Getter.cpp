#include "Getter.h"
// c++
#include <cassert>
// Engine
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
// Manager
#include "Engine/System/Managers/SRVManager.h"
#include "Engine/System/Managers/TextureManager.h"
#include "Engine/System/Managers/ModelManager.h"
#include "Engine/System/Managers/AnimationManager.h"

/// ===宣言=== ///
// Engine
WinApp* Getter::winApp_ = nullptr;
DXCommon* Getter::dXCommon_ = nullptr;
// Manager
SRVManager* Getter::srvManager_ = nullptr;
TextureManager* Getter::textureManager_ = nullptr;
ModelManager* Getter::modelManager_ = nullptr;
AnimationManager* Getter::animationManager_ = nullptr;

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Getter::Initialize(
	DXCommon* dxCommon, WinApp* winApp, SRVManager* srvManager, ModelManager* modelManager, TextureManager* textureManager, AnimationManager* animationManager) {
	assert(dxCommon);
	assert(winApp);
	assert(srvManager);
	assert(modelManager);
	assert(textureManager);
	assert(animationManager);

	// 生成
	dXCommon_ = dxCommon;
	winApp_ = winApp;
	srvManager_ = srvManager;
	modelManager_ = modelManager;
	textureManager_ = textureManager;
	animationManager_ = animationManager;
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void Getter::Finalize() {
	dXCommon_ = nullptr;
	winApp_ = nullptr;
	srvManager_ = nullptr;
	modelManager_ = nullptr;
	textureManager_ = nullptr;
	animationManager_ = nullptr;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
#pragma region Device
ID3D12Device* Getter::GetDXDevice() { return dXCommon_->GetDevice(); }
#pragma endregion
#pragma region GetDescriptorHandle
// RTV
D3D12_CPU_DESCRIPTOR_HANDLE Getter::GetRTVCPUDescriptorHandle(uint32_t index) { return dXCommon_->GetRTVCPUDescriptorHandle(index); }
D3D12_GPU_DESCRIPTOR_HANDLE Getter::GetRTVGPUDescriptorHandle(uint32_t index) { return dXCommon_->GetRTVGPUDescriptorHandle(index); }
// DSV
D3D12_CPU_DESCRIPTOR_HANDLE Getter::GetDSVCPUDescriptorHandle(uint32_t index) { return dXCommon_->GetDSVCPUDescriptorHandle(index); }
D3D12_GPU_DESCRIPTOR_HANDLE Getter::GetDSVGPUDescriptorHandle(uint32_t index) { return dXCommon_->GetDSVGPUDescriptorHandle(index); }
// SRV
D3D12_CPU_DESCRIPTOR_HANDLE Getter::GetSRVCPUDescriptorHandle(uint32_t index) { return srvManager_->GetCPUDescriptorHandle(index); }
D3D12_GPU_DESCRIPTOR_HANDLE Getter::GetSRVGPUDescriptorHandle(uint32_t index) { return srvManager_->GetGPUDescriptorHandle(index); }
#pragma endregion
#pragma region GetScreenSize
///-------------------------------------------/// 
/// スクリーンサイズ
///-------------------------------------------///
const int Getter::GetWindowWidth() { return winApp_->GetWindowWidth(); }
const int Getter::GetWindowHeight() { return winApp_->GetWindowHeight(); }
#pragma endregion
#pragma region Texture
// メタデータの取得
const DirectX::TexMetadata& Getter::GetMetaData(const std::string& key) { return textureManager_->GetMetaData(key); }
#pragma endregion
#pragma region Model
// モデルデータの取得
ModelData Getter::GetModelData(const std::string& directorPath) { return modelManager_->GetModelData(directorPath); }
#pragma endregion
#pragma region Animation
// アニメーションの取得
Animation Getter::GetAnimationData(const std::string& directorPath) { return animationManager_->GetAnimation(directorPath); }
#pragma endregion