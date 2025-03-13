#include "RTVManager.h"
// Engine
#include "Engine/Core/DXCommon.h"

/// ===定数=== ///
const uint32_t RTVManager::kNumRTVDescriptor_ = 2;

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
RTVManager::RTVManager() {}
RTVManager::~RTVManager() {
	descriptorHeap_.Reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// DescriptorHeap
ID3D12DescriptorHeap* RTVManager::GetDescriptorHeap() const { return descriptorHeap_.Get(); }
// CPUHandl
D3D12_CPU_DESCRIPTOR_HANDLE RTVManager::GetCPUDescriptorHandle(uint32_t index){
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}
// GPUHandl
D3D12_GPU_DESCRIPTOR_HANDLE RTVManager::GetGPUDescriptorHandle(uint32_t index){
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

///-------------------------------------------/// 
/// 生成
///-------------------------------------------///
void RTVManager::CreateFinalRenderTargets(DXCommon* dxcommon, ID3D12Resource* backBuffers[], uint32_t backBufferCount) {
	HRESULT hr;

	// RTVの生成(レンダーターゲットビュー)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー用
	heapDesc.NumDescriptors = kNumRTVDescriptor_; // ダブルバッファ用に2つ以上
	heapDesc.Flags = false ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	// ディスクリプタヒープを作成
	hr = dxcommon->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap_));
	// ディスクリプターヒープが作れなかったので起動できない
	assert(SUCCEEDED(hr));

	// RTVのサイズを取得
	const uint32_t rtvSize = dxcommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// RTVの設定(レンダーターゲットビュー)
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;  // 出力結果をSRGBに変換して書き込む
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;  // 2dテクスチャとして書き込む

	// まず1つ目を作る。1つ目は最初のところに作る。作る場所をコリらで指定してあげる必要がある
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (rtvSize * 0);
	descriptorHandles_[0] = handleCPU;
	dxcommon->GetDevice()->CreateRenderTargetView(backBuffers[0], &desc, descriptorHandles_[0]);

	//2つ目のディスクリプタハンドルを得る
	descriptorHandles_[1].ptr = descriptorHandles_[0].ptr + dxcommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//2つ目を作る
	dxcommon->GetDevice()->CreateRenderTargetView(backBuffers[1], &desc, descriptorHandles_[1]);
}


