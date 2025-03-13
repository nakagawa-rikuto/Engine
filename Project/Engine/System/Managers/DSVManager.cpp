#include "DSVManager.h"
// Engine
#include "Engine/Core/DXCommon.h"
// Service
#include "Engine/System/Service/Getter.h"

/// ===定数=== ///
const uint32_t DSVManager::kNumDSVDescriptor_ = 1;

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
DSVManager::DSVManager() {}
DSVManager::~DSVManager() {}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Heap
ID3D12DescriptorHeap* DSVManager::GetDescriptorHeap() const { return descriptorHeap_.Get(); }
// CPUHandle
D3D12_CPU_DESCRIPTOR_HANDLE DSVManager::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}
// GPUHandle
D3D12_GPU_DESCRIPTOR_HANDLE DSVManager::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

///-------------------------------------------/// 
/// 生成
///-------------------------------------------///
void DSVManager::CreateDepthBuffer(DXCommon* dxcommon, ID3D12Resource* depthStencil) {
	HRESULT hr;

	// DescriptorSizeの取得, Heapの生成
	descriptorHeap_ = dxcommon->CreateDSVHeap();
	descriptorSize_ = dxcommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// 生成するDSVの設定
	D3D12_RESOURCE_DESC desc{};
	desc.Width = Getter::GetWindowWidth(); // Textureの幅
	desc.Height = Getter::GetWindowHeight(); // Textureの高さ
	desc.MipLevels = 1; // mipmapの数
	desc.DepthOrArraySize = 1; // 奥行 or 配列Textureの配列数
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして利用可能なフォーマット
	desc.SampleDesc.Count = 1; // サンプリングカウント
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。

	// DepthStencilTextureをウィンドウのサイズで作成
	hr = dxcommon->GetDevice()->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // HEapの特殊な設定
		&desc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&depthStencilResource_)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture

	// DSVHeapの先頭にDSVをつくる
	dxcommon->GetDevice()->CreateDepthStencilView(
		depthStencilResource_.Get(), &dsvDesc, descriptorHeap_->GetCPUDescriptorHandleForHeapStart());

	// 描画先のRTVとDSVを設定する
	descriptorHandles_ = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
}

