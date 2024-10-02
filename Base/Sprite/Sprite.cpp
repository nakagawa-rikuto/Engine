#include "Sprite.h"

#include <cassert>

#include "WinApp.h"
#include "SpriteCommon.h"
#include "sMath.h"

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
Sprite* Sprite::GetInstance() {
	static Sprite instance;
	return &instance;
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Sprite::Initialize(SpriteCommon* spriteCommon) {

	// 引数で受け取ってメンバ変数に記録する
	this->spriteCommon_ = spriteCommon;

	// Resourceの作成
	vertexBuffer_ = CreateResource(spriteCommon_->GetDXCommon()->GetDevice(), sizeof(VertexData) * 6);
	indexBuffer_ = CreateResource(spriteCommon_->GetDXCommon()->GetDevice(), sizeof(uint32_t) * 6);
	materialBuffer_ = CreateResource(spriteCommon_->GetDXCommon()->GetDevice(), sizeof(MaterialData) * 3);
	wvpBuffer_ = CreateResource(spriteCommon_->GetDXCommon()->GetDevice(), sizeof(TransformationMatrix));

	// ResourceViewの作成
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress(); // 先頭アドレスから使用
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6; // 使用するサイズ（頂点6つ分）
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // １頂点当たりのサイズ

	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress(); // 先頭のアドレスから使用
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6; // 使用するサイズ（６つ分）
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT; // uint32_tとする

	// 書き込むためのアドレスを取得
	vertexBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&vertexData_));
	indexBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&indexData_));
	materialBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&materialData_));
	wvpBuffer_->Map(
		0, nullptr, reinterpret_cast<void**>(&wvpMatrixData_));

	// マテリアルデータの初期値を書き込む
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();

	// 単位行列を書き込んでおく
	wvpMatrixData_->WVP = MakeIdentity4x4();
	wvpMatrixData_->World = MakeIdentity4x4();

	// TransformInfoの設定
	transform_ = { {1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, }, { 0.0f, 0.0f, 0.0f } };
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Sprite::Update() {

	VertexDataWrite();
	IndexDataWrite();
	TransformDataWrite();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Sprite::Draw() {

	// VertexBufferViewの設定
	spriteCommon_->GetDXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// IndexBufferViewの設定
	spriteCommon_->GetDXCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	// Materialの設定
	spriteCommon_->GetDXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());

	// wvpMatrixBufferの設定
	spriteCommon_->GetDXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpBuffer_->GetGPUVirtualAddress());

	// テクスチャの設定
	//spriteCommon_->GetDXCommon()->GetCommandList()->SetGraphicsRootDescriptorTable()

	// 描画(ドローコール)
	spriteCommon_->GetDXCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
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

///-------------------------------------------/// 
/// VertexResourceの書き込み
///-------------------------------------------///
void Sprite::VertexDataWrite() {

	vertexData_[0].position = { 0.0f, 180.0f, 0.0f, 1.0f };
	vertexData_[0].texcoord = { 0.0f, 1.0f };
	vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };

	vertexData_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertexData_[1].texcoord = { 0.0f, 0.0f };
	vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };

	vertexData_[2].position = { 320.0f, 180.0f, 0.0f, 1.0f };
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };

	vertexData_[3].position = { 320.0f, 0.0f, 0.0f, 1.0f };
	vertexData_[3].texcoord = { 1.0f, 0.0f };
	vertexData_[3].normal = { 0.0f, 0.0f, -1.0f };
}

///-------------------------------------------/// 
/// IndexResourceの書き込み
///-------------------------------------------///
void Sprite::IndexDataWrite() {

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;
}

///-------------------------------------------/// 
/// TransformDataの書き込み
///-------------------------------------------///
void Sprite::TransformDataWrite() {

	// WorldMatrix
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	// ViewMatrix
	Matrix4x4 viewMatrix = MakeIdentity4x4();

	// ProjectionMatrix
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, static_cast<float>(WinApp::GetWindowWidth()), static_cast<float>(WinApp::GetWindowHeight()), 0.0f, 100.0f);

	wvpMatrixData_->WVP = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	wvpMatrixData_->World = worldMatrix;

}