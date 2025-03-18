#include "RTVManager.h"
// Engine
#include "Engine/Core/DXCommon.h"

/// ===定数=== ///
const uint32_t RTVManager::kNumRTVDescriptor_ = 2;

///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
RTVManager::RTVManager() = default;
RTVManager::~RTVManager() {
	descriptorHeap_.Reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Haneld
D3D12_CPU_DESCRIPTOR_HANDLE RTVManager::GetHandle(uint32_t index) const { return descriptorHandles_[index]; }
// Heap
ID3D12DescriptorHeap* RTVManager::GetDescriptorHeap() const { return descriptorHeap_.Get(); }
// CPU
D3D12_CPU_DESCRIPTOR_HANDLE RTVManager::GetCPUDescriptorHandle(uint32_t index) const { return dxcommon_->GetCPUDescriptorHandle(descriptorHeap_, descriptorSize_, index); }
// GPU
D3D12_GPU_DESCRIPTOR_HANDLE RTVManager::GetGPUDescriptorHandle(uint32_t index) const { return dxcommon_->GetGPUDescriptorHandle(descriptorHeap_, descriptorSize_, index); }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void RTVManager::Initialize(DXCommon* dxcommon) {
	dxcommon_ = dxcommon;
	// RTVの作成
	CreateFinalRenderTargets();
}

///-------------------------------------------/// 
/// 作成
///-------------------------------------------///
void RTVManager::CreateFinalRenderTargets() {
	// RTVの生成(レンダーターゲットビュー)
	descriptorHeap_ = dxcommon_->CreateRTVHeap(kNumRTVDescriptor_); // この関数は変更
	descriptorSize_ = dxcommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// RTVの設定(レンダーターゲットビュー)
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;  // 出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;  // 2dテクスチャとして書き込む

	// まず1つ目を作る。1つ目は最初のところに作る。作る場所をコリらで指定してあげる必要がある
	descriptorHandles_[0] = dxcommon_->GetCPUDescriptorHandle(descriptorHeap_.Get(), descriptorSize_, 0);
	dxcommon_->GetDevice()->CreateRenderTargetView(dxcommon_->GetSwapChainResource(0), &rtvDesc, descriptorHandles_[0]);

	//2つ目のディスクリプタハンドルを得る
	descriptorHandles_[1].ptr = descriptorHandles_[0].ptr + dxcommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//2つ目を作る
	dxcommon_->GetDevice()->CreateRenderTargetView(dxcommon_->GetSwapChainResource(1), &rtvDesc, descriptorHandles_[1]);
}

///-------------------------------------------/// 
/// クリア
///-------------------------------------------///
void RTVManager::ClearRenderTarget(ID3D12GraphicsCommandList * commandList) {

	UINT backBufferIndex = dxcommon_->GetBackBufferIndex();

	// 指定した色で画面全体をクリアする
	float cleaerColor[] = { 0.1f, 0.25f, 0.5f, 1.0f }; // 青色っっぽい色、RGBAの順
	commandList->ClearRenderTargetView(descriptorHandles_[backBufferIndex], cleaerColor, 0, nullptr);
}

