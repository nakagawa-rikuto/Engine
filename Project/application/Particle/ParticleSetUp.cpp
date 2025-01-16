#include "ParticleSetUp.h"

// Math
#include "Math/sMath.h"
// c++
#include <cassert>

///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
ParticleSetUp::ParticleSetUp() = default;
ParticleSetUp::~ParticleSetUp() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleSetUp::Initlize(ID3D12Device* device, const uint32_t kNumMaxInstance) {
	// 生成
	Create(device, sizeof(ParticleForGPU) * kNumMaxInstance);
	// 書き込むためのアドレスを取得
	buffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	
	// Dataの書き込み
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		data_[index].WVP = MakeIdentity4x4();
		data_[index].World = MakeIdentity4x4();
		data_[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
ID3D12Resource* ParticleSetUp::GetBuffer() { return buffer_.Get(); }

///-------------------------------------------/// 
/// Bufferの生成
///-------------------------------------------///
void ParticleSetUp::Create(ID3D12Device* device, size_t sizeInBytes) {
	HRESULT hr;

	// リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;  // UploadHeapを使う

	// リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc{};

	// バッファリソース。テクスチャの場合はまた別の設定をする
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInBytes; // リソースのサイズ。今回はVector4を3頂点分

	// バッファの場合はこれらは1にする決まり
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;

	// バッファの場合はこれにする決まり
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 実際に頂点リソースを作る
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer_));
	assert(SUCCEEDED(hr));
}
