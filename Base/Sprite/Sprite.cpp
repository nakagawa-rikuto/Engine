#include "Sprite.h"

#include <cassert>

#include "SpriteCommon.h"

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
Sprite* Sprite::GetInstance() {
	static Sprite instance;
	return &instance;
}

void Sprite::Initialize(SpriteCommon* spriteCommon) {

	// 引数で受け取ってメンバ変数に記録する
	this->spriteCommon_ = spriteCommon;

	// Resourceの作成
	vertexBuffer_ = CreateResource(spriteCommon_->GetDXCommon()->GetDevice(), sizeof(vertexData_) * 6);
	indexBuffer_ = CreateResource(spriteCommon_->GetDXCommon()->GetDevice(), sizeof(indexData_) * 6);

	// ResourceViewの作成
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress(); // 先頭アドレスから使用
	vertexBufferView_.SizeInBytes = sizeof(vertexData_) * 6; // 使用するサイズ（頂点6つ分）
	vertexBufferView_.StrideInBytes = sizeof(vertexData_); // １頂点当たりのサイズ

	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress(); // 先頭のアドレスから使用
	indexBufferView_.SizeInBytes = sizeof(indexData_) * 6; // 使用するサイズ（６つ分）
	indexBufferView_.StrideInBytes = DXGI_FORMAT_R32_UINT; // uint32_tとする

	// 書き込むためのアドレスを取得
	vertexBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&vertexData_));
	indexBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&indexData_));
}

///-------------------------------------------/// 
/// Resourceの作成関数
///-------------------------------------------///
Microsoft::WRL::ComPtr<ID3D12Resource> Sprite::CreateResource(ID3D12Device* device, size_t sizeInBytes) {
	HRESULT hr;

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;  // UploadHeapを使う

	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};

	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; // リソースのサイズ。今回はVector4を3頂点分

	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;

	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));

	return Resource;
}
