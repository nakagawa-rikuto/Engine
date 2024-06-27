#include "Material.h"

#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <format>

/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
Material* Material::GetInstance() {
    static Material instance;
    return &instance;
}

/// <summary>
/// リソースの生成
/// </summary>
void Material::CreateMaterial(ID3D12Device* device, size_t sizeInBytes) {
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
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&bufferResource_));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// マテリアルデータの書き込み
/// </summary>
void Material::WriteMaterial(MaterialData* material) {
	// 書き込むためのアドレスを取得
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

	// 色の書き込み
	data_->color = material->color;
}

/// <summary>
/// リソースの取得
/// </summary>
ID3D12Resource* Material::GetResource() { return bufferResource_.Get(); }
