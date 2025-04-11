#include "RenderTarget.h"
// Engine
#include "Engine/System/Managers/RTVManager.h"
#include "Engine/System/Managers/SRVManager.h"
// Service
#include "Engine/System/Service/ServiceLocator.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void RenderTarget::Initialize(uint32_t width, uint32_t height, const Vector4& color) {

	width_ = width;
	height_ = height;
	format_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearColor_ = color;
	// Managerの取得
	rtvManager_ = ServiceLocator::GetRTVManager();
	srvManager_ = ServiceLocator::GetSRVManager();
}

///-------------------------------------------///  
/// RenderTargetの生成
///-------------------------------------------///
void RenderTarget::CreateRenderTexture(ID3D12Device* device) {
	
	/// ===Resource=== ///
	// DESCの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // RenderTargetとして利用可能にする

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // 当然VRAM状に作る

	// ClearValueの設定
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format_; // フォーマット
	clearValue.Color[0] = clearColor_.x; // RGBAの値
	clearValue.Color[1] = clearColor_.y;
	clearValue.Color[2] = clearColor_.z;
	clearValue.Color[3] = clearColor_.w;

	// Resourceの生成
	device->CreateCommittedResource(
		&heapProperties,			// Heapの設定
		D3D12_HEAP_FLAG_NONE,		// Heapのフラグ
		&resourceDesc,				// ResourceのDESC
		D3D12_RESOURCE_STATE_RENDER_TARGET, // Resourceの状態
		&clearValue,				// ClearValueの設定
		IID_PPV_ARGS(&buffer_));	// Resourceのポインタ


	/// ===RTV=== ///
	// DESCの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = format_; // フォーマット
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2Dテクスチャ

	// RTVの作成
	rtvHandleIndex_ = rtvManager_->Allocate(); // RTVのインデックスを確保
	rtvManager_->CreateRenderTarget(rtvHandleIndex_, buffer_.Get(), rtvDesc);


	/// ===SRV=== ///
	// DESCの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format_; // フォーマット
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// SRVの作成
	srvHandleIndex_ = srvManager_->Allocate(); // SRVのインデックスを確保
	srvManager_->CreateSRV(srvHandleIndex_, buffer_.Get(), srvDesc);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// RenderTargetの取得
ID3D12Resource* RenderTarget::GetBuffer() { return buffer_.Get(); }
// RTVHandleの取得
D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetRTVHandle() { return rtvManager_->GetCPUDescriptorHandle(rtvHandleIndex_); }
// SRVHandleの取得
D3D12_GPU_DESCRIPTOR_HANDLE RenderTarget::GetSRVHandle() { return srvManager_->GetGPUDescriptorHandle(srvHandleIndex_); }
// RTVのインデックス番号の取得
uint32_t RenderTarget::GetRTVHandleIndex() const { return rtvHandleIndex_; }
// SRVのインデックス番号の取得
uint32_t RenderTarget::GetSRVHandleIndex() const { return srvHandleIndex_; }
