#include "Sprite.h"

#include "sMath.h"

/* ////////////////////////////////////////////////////////////
					シングルトンインスタンス
*/ ////////////////////////////////////////////////////////////
Sprite* Sprite::GetInstance() {
	static Sprite instance;
	return &instance;
}

/* /////////////////////////////////////////////////////////////////////
								描画（前・後）処理
*/ /////////////////////////////////////////////////////////////////////
void Sprite::PreDraw() {
	dxCommon_->GetInstance();
}

void Sprite::PostDraw() {

}

/* ////////////////////////////////////////////////////////////
					   VertexBufferの生成
*/ ////////////////////////////////////////////////////////////
void Sprite::CreateVertexBuffer(ID3D12Device* device, size_t sizeInBytes) {
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

/* ////////////////////////////////////////////////////////////
					   IndexBufferの生成
*/ ////////////////////////////////////////////////////////////
void Sprite::CreateIndexBuffer(ID3D12Device* device, size_t sizeInBytes) {
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
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBuffer_));
	assert(SUCCEEDED(hr));
}

/* ////////////////////////////////////////////////////////////
					 三角形のデータの書き込み
*/ ////////////////////////////////////////////////////////////
void Sprite::WriteTriangle(
	Vector2* leftBottom,
	Vector2* Top,
	Vector2* rightBottom) {

	VertexData* vertexData; // 頂点リソース
	uint32_t* indexData;    // インデックスリソース

	// 書き込むためのアドレスを取得
	vertexBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&vertexData));
	indexBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&indexData));

	// 左下
	vertexData[0].position = { leftBottom->x, leftBottom->y, 0.0f, 1.0f };
	vertexData[0].texcoord = {0.0f, 1.0f};
	vertexData[0].normal = {0.0f, 0.0f, -1.0f};

	// 上
	vertexData[1].position = { Top->x, Top->y, 0.0f, 1.0f };
	vertexData[1].texcoord = {0.5f, 0.0f};
	vertexData[1].normal = {0.0f, 0.0f, -1.0f};

	// 右下
	vertexData[2].position = { rightBottom->x, rightBottom->y, 0.0f, 1.0f };
	vertexData[2].texcoord = {1.0f, 1.0f};
	vertexData[2].normal = {0.0f, 0.0f, -1.0f};

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	// マップしたバッファをアンマップする
	//vertexBuffer_->Unmap(0, nullptr);
	//indexBuffer_->Unmap(0, nullptr);
}

/* ////////////////////////////////////////////////////////////
					 四角形のデータの書き込み
*/ ////////////////////////////////////////////////////////////
void Sprite::WriteSquare(
	Vector2* leftTop,
	Vector2* rightTop,
	Vector2* leftBottom,
	Vector2* rightBottom) {

	VertexData* vertexData; // 頂点リソース
	uint32_t* indexData;    // インデックスリソース

	// 書き込むためのアドレスを取得
	vertexBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&vertexData));
	indexBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&indexData));

	// 頂点データの設定
	// 左上
	vertexData[0].position = { leftTop->x, leftTop->y, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.0f, 0.0f }; // テクスチャの左上
	vertexData[0].normal = { 0.0f, 0.0f, 1.0f }; // 法線が前方（Z+）を向く

	// 右上
	vertexData[1].position = { rightTop->x, rightTop->y, 0.0f, 1.0f };
	vertexData[1].texcoord = { 1.0f, 0.0f }; 
	vertexData[1].normal = { 0.0f, 0.0f, 1.0f };

	// 左下
	vertexData[2].position = { leftBottom->x, leftBottom->y, 0.0f, 1.0f };
	vertexData[2].texcoord = { 0.0f, 1.0f }; 
	vertexData[2].normal = { 0.0f, 0.0f, 1.0f };

	// 右下
	vertexData[3].position = { rightBottom->x, rightBottom->y, 0.0f, 1.0f };
	vertexData[3].texcoord = { 1.0f, 1.0f }; 
	vertexData[3].normal = { 0.0f, 0.0f, 1.0f };

	// インデックスデータの設定
	indexData[0] = 0;  // 左上
	indexData[1] = 1;  // 右上
	indexData[2] = 2;  // 左下
	indexData[3] = 1;  // 右上
	indexData[4] = 3;  // 右下
	indexData[5] = 2;  // 左下

	// マップしたバッファをアンマップする
	//vertexBuffer_->Unmap(0, nullptr);
	//indexBuffer_->Unmap(0, nullptr);
}

/* ////////////////////////////////////////////////////////////
					       Getter
*/ ////////////////////////////////////////////////////////////
// Bufferの所得
ID3D12Resource* Sprite::GetVertexBuffer() {return vertexBuffer_.Get();}
ID3D12Resource* Sprite::GetIndexBuffer() {return indexBuffer_.Get();}

// BufferViewの取得
D3D12_VERTEX_BUFFER_VIEW Sprite::GetVertexBufferView() {return vertexBufferView_;}
D3D12_INDEX_BUFFER_VIEW Sprite::GetIndexBufferView() {return indexBufferView_;}
