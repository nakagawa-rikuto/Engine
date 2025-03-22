#include "Getter.h"
// c++
#include <cassert>
// Engine
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
// Manager
#include "Engine/System/Managers/SRVManager.h"
#include "Engine/System/Managers/RTVManager.h"
#include "Engine/System/Managers/DSVManager.h"
#include "Engine/System/Managers/TextureManager.h"
#include "Engine/System/Managers/ModelManager.h"
#include "Engine/System/Managers/AnimationManager.h"
// ServiceLocator
#include "ServiceLocator.h"

///-------------------------------------------/// 
/// DescriptorHandle
///-------------------------------------------///
// RTV
D3D12_CPU_DESCRIPTOR_HANDLE Getter::GetRTVCPUDescriptorHandle(uint32_t index) { 
	return ServiceLocator::GetRTVManager()->GetCPUDescriptorHandle(index);
}
D3D12_GPU_DESCRIPTOR_HANDLE Getter::GetRTVGPUDescriptorHandle(uint32_t index) { 
	return ServiceLocator::GetRTVManager()->GetGPUDescriptorHandle(index);
}
// DSV
D3D12_CPU_DESCRIPTOR_HANDLE Getter::GetDSVCPUDescriptorHandle(uint32_t index) { 
	return ServiceLocator::GEtDSVManager()->GetCPUDescriptorHandle(index);
}
D3D12_GPU_DESCRIPTOR_HANDLE Getter::GetDSVGPUDescriptorHandle(uint32_t index) { 
	return ServiceLocator::GEtDSVManager()->GetGPUDescriptorHandle(index);
}
// SRV
D3D12_CPU_DESCRIPTOR_HANDLE Getter::GetSRVCPUDescriptorHandle(uint32_t index) { 
	return ServiceLocator::GetSRVManager()->GetCPUDescriptorHandle(index);
}
D3D12_GPU_DESCRIPTOR_HANDLE Getter::GetSRVGPUDescriptorHandle(uint32_t index) { 
	return ServiceLocator::GetSRVManager()->GetGPUDescriptorHandle(index);
}

///-------------------------------------------/// 
/// WinApp
///-------------------------------------------///
const int Getter::GetWindowWidth() { 
	return ServiceLocator::GetWinApp()->GetWindowWidth();
}
const int Getter::GetWindowHeight() { 
	return ServiceLocator::GetWinApp()->GetWindowHeight();
}

///-------------------------------------------/// 
/// Data
///-------------------------------------------///
// メタデータの取得
const DirectX::TexMetadata& Getter::GetMetaData(const std::string& key) { 
	return ServiceLocator::GetTextureManager()->GetMetaData(key);
}
// モデルデータの取得
ModelData Getter::GetModelData(const std::string& directorPath) { 
	return ServiceLocator::GetModelManager()->GetModelData(directorPath);
}
// アニメーションの取得
std::map<std::string, Animation> Getter::GetAnimationData(const std::string& directorPath) {
	return ServiceLocator::GetAnimationManager()->GetAnimation(directorPath);
}

///-------------------------------------------/// 
/// DXCommon
///-------------------------------------------///
// Deviceの取得
ID3D12Device* Getter::GetDXDevice() { 
	return ServiceLocator::GetDXCommon()->GetDevice();
}
// CommandListの取得
ID3D12GraphicsCommandList* Getter::GetDXCommandList() { 
	return ServiceLocator::GetDXCommon()->GetCommandList();
}