#include "IndexBuffer3D.h"

#include <cassert>

IndexBuffer3D::IndexBuffer3D() {}
IndexBuffer3D::~IndexBuffer3D() {}

///-------------------------------------------/// 
///リソースの作成
///-------------------------------------------///
void IndexBuffer3D::Create(ID3D12Device* device, size_t sizeInBytes) {
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

///-------------------------------------------/// 
/// データの書き込み
///-------------------------------------------///
void IndexBuffer3D::WriteData(uint32_t* data) {
	// 書き込むためのアドレスを取得
	buffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

	data_ = data;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
ID3D12Resource* IndexBuffer3D::GetBuffer() { return buffer_.Get(); }
uint32_t* IndexBuffer3D::GetData() { return data_; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void IndexBuffer3D::SetData(uint32_t* data) { data_ = data; }