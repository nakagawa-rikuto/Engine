#include "Mesh.h"

#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <format>

/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
Mesh* Mesh::GetInstance() {
	static Mesh instance;
	return &instance;
}

/// <summary>
/// VertexResourceの生成
/// </summary>
void Mesh::CreateVertexResource(ID3D12Device* device, size_t sizeInBytes) {
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
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer_));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// 三角形のデータの書き込み
/// </summary>
void Mesh::WriteTriangleData(
	VertexData* TriangleLeftBottomPositionData, VertexData* TriangleTopPositionData, VertexData* TriangleRightBottomPositionData) {

	// 頂点リソース
	VertexData* data;

	// 書き込むためのアドレスを取得
	vertexBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&data));

	// 左下
	data[0].position = TriangleLeftBottomPositionData->position;

	// 上
	data[1].position = TriangleTopPositionData->position;

	// 右下
	data[2].position = TriangleRightBottomPositionData->position;
}

/// <summary>
/// VertexBufferの取得
/// </summary>
ID3D12Resource* Mesh::GetVertexBuffer() {return vertexBuffer_.Get();}

/// <summary>
/// VertexBufferViewの取得
/// </summary>
D3D12_VERTEX_BUFFER_VIEW Mesh::GetVertexBufferView() { return vertexBufferView_;}
