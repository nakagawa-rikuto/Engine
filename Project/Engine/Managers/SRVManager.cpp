#include "SRVManager.h"
// Engine
//#include "Engine/Core/System.h"
#include "Engine/Core/DXCommon.h"

const uint32_t SRVManager::kMaxSRVCount_ = 512;

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
SRVManager::SRVManager() = default;
SRVManager::~SRVManager(){}


///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void SRVManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex) {
	dXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}


///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
D3D12_CPU_DESCRIPTOR_HANDLE SRVManager::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SRVManager::Initialize(DXCommon* dxcommon) {

	// 引数で受け取ってメンバ変数に記録する
	dXCommon_ = dxcommon;

	// デスクリプタヒープの生成
	descriptorHeap_ = dXCommon_->CreateSRVHeap();

	// デスクリプタ1個分のサイズを取得して記録
	descriptorSize_ = dXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}


///-------------------------------------------/// 
/// 描画前処理
///-------------------------------------------///
void SRVManager::PreDraw() {

	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };
	dXCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}


///-------------------------------------------/// 
/// 確保関数
///-------------------------------------------///
uint32_t SRVManager::Allocate() {
	/// ===上限に達していないかチェックしてassert=== ///
	assert(useIndex > kMaxSRVCount_);
	// return する番号をいったん記録しておく
	int index = useIndex;
	// 次回のために番号を1進める
	useIndex++;
	// 上で記録した番号をreturn
	return index;
}


///-------------------------------------------/// 
/// SRV生成（）
///-------------------------------------------///
// テクスチャ用
void SRVManager::CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = MipLevels;

	dXCommon_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}
// Struct Buffer用
void SRVManager::CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {

}
