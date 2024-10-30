#include "Sprite.h"

#include <cassert>

#include "Base/System/System.h"
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DXCommon.h"

#include "Base/TextureManager/TextureManager.h"

#include "Base/Math/sMath.h"

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// 座標
const Vector2& Sprite::GetPosition() const { return position_; }
// 回転
const float& Sprite::GetRotation() const { return rotation_; }
// サイズ
const Vector2& Sprite::GetSize() const { return size_; }
// 色
const Vector4& Sprite::GetColor() const { return color_; }


///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// 座標
void Sprite::SetPosition(const Vector2& position) { this->position_ = position; }
// 回転
void Sprite::SetRotation(const float& rotation) { this->rotation_ = rotation; }
// サイズ
void Sprite::SetSize(const Vector2& size) { this->size_ = size; }
// 色
void Sprite::SetColor(const Vector4& color) { this->color_ = color; }


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Sprite::Initialize(std::string textureFilePath) {

	// コマンドリストのポインタ
	ID3D12Device* device = System::GetDXDevice();

	// Resourceの生成
	vertex_ = std::make_unique<VertexBuffer2D>();
	index_ = std::make_unique<IndexBuffer2D>();
	material_ = std::make_unique<Material2D>();
	wvp_ = std::make_unique<Transform2D>();
	// Resourceの作成
	vertex_->Create(device, sizeof(VertexData2D) * 6);
	index_->Create(device, sizeof(uint32_t) * 6);
	material_->Create(device, sizeof(MaterialData2D) * 3);
	wvp_->Create(device, sizeof(TransformationMatrix2D));

	// ResourceViewの作成
	vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress(); // 先頭アドレスから使用
	vertexBufferView_.SizeInBytes = sizeof(VertexData2D) * 6; // 使用するサイズ（頂点6つ分）
	vertexBufferView_.StrideInBytes = sizeof(VertexData2D); // １頂点当たりのサイズ

	indexBufferView_.BufferLocation = index_->GetBuffer()->GetGPUVirtualAddress(); // 先頭のアドレスから使用
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6; // 使用するサイズ（６つ分）
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT; // uint32_tとする

	// 書き込むためのアドレスを取得
	vertex_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	index_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	material_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	wvp_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&wvpMatrixData_));

	/*vertex_->WriteData(vertexData_);
	index_->WriteData(indexData_);
	material_->WriteData(materialData_);
	wvp_->WriteData(wvpMatrixData_);*/

	// マテリアルデータの初期値を書き込む
	materialData_->color = color_;
	materialData_->uvTransform = MakeIdentity4x4();

	// 単位行列を書き込んでおく
	wvpMatrixData_->WVP = MakeIdentity4x4();

	// Dataの書き込み
	VertexDataWrite();
	IndexDataWrite();

	// TransformInfoの設定
	worldTransform_ = { {1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, }, { 0.0f, 0.0f, 0.0f } };

	// 単位行列を書き込んでおく
	textureIndex = System::GetTextureIndexByFilePath(textureFilePath);

	color_.x = 0.0;
}


///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Sprite::Update() {

	TransformDataWrite();

	color_.x += 0.03f;

	materialData_->color = color_;
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Sprite::Draw() {

	// コマンドリストのポインタ
	ID3D12GraphicsCommandList* commandList = System::GetDXCommandList();

	// VertexBufferViewの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// IndexBufferViewの設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// Materialの設定
	commandList->SetGraphicsRootConstantBufferView(0, material_->GetBuffer()->GetGPUVirtualAddress());

	// wvpMatrixBufferの設定
	commandList->SetGraphicsRootConstantBufferView(1, wvp_->GetBuffer()->GetGPUVirtualAddress());

	D3D12_GPU_DESCRIPTOR_HANDLE handle = System::GetSRVHandleGPU(textureIndex);
	// テクスチャの設定
	commandList->SetGraphicsRootDescriptorTable(2, System::GetSRVHandleGPU(textureIndex));

	// 描画(ドローコール)
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
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

	vertexData_[0].position = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertexData_[0].texcoord = { 0.0f, 1.0f };

	vertexData_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertexData_[1].texcoord = { 0.0f, 0.0f };

	vertexData_[2].position = { 1.0f, 1.0f, 0.0f, 1.0f };
	vertexData_[2].texcoord = { 1.0f, 1.0f };

	vertexData_[3].position = { 1.0f, 0.0f, 0.0f, 1.0f };
	vertexData_[3].texcoord = { 1.0f, 0.0f };
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

	// 座標の反映
	worldTransform_.translate = { position_.x, position_.y, 0.0f };
	// 回転の反映
	worldTransform_.rotate = { 0.0, 0.0, rotation_ };
	// サイズの反映
	worldTransform_.scale = { size_.x, size_.y, 1.0f };

	// WorldMatrix
	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	// ViewMatrix
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	// ProjectionMatrix
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, static_cast<float>(WinApp::GetWindowWidth()), static_cast<float>(WinApp::GetWindowHeight()), 0.0f, 100.0f);

	wvpMatrixData_->WVP = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
}