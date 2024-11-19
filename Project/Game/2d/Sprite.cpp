#include "Sprite.h"
// Engine
#include "Engine/Core/System.h"
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
// Math
#include "Math/sMath.h"
// c++
#include <cassert>

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
Sprite::Sprite() = default;
Sprite::~Sprite() {
	vertex_.reset();
	index_.reset();
	material_.reset();
	wvp_.reset();
	pipelineCommon_.reset();
}


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
void Sprite::SetPosition(const Vector2& position) { position_ = position; }
// 回転
void Sprite::SetRotation(const float& rotation) { rotation_ = rotation; }
// サイズ
void Sprite::SetSize(const Vector2& size) { size_ = size; }
// 色
void Sprite::SetColor(const Vector4& color) { color_ = color; }
// アンカーポイント
void Sprite::SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
// フリップ
void Sprite::SetFlipX(const bool& flip) { isFlipX_ = flip; }
void Sprite::SetFlipY(const bool& flip) { isFlipY_ = flip; }
// テクスチャ
void Sprite::SetTexture(std::string textureFilePath) {
	textureIndex = System::GetTextureIndexByFilePath(textureFilePath);
	AdjustTextureSize();
	isLoadTexture_ = true;
}
// テクスチャ左上座標
void Sprite::SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }
// テクスチャ切り出しサイズ
void Sprite::SetTextureSize(const Vector2& textureSize) { textureSize_ = textureSize; }


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Sprite::Initialize(BlendMode mode) {

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12Device* device = System::GetDXDevice();

	/// ===生成=== ///
	vertex_ = std::make_unique<VertexBuffer2D>();
	index_ = std::make_unique<IndexBuffer2D>();
	material_ = std::make_unique<Material2D>();
	wvp_ = std::make_unique<Transform2D>();

	/// ===vertex=== ///
	// buffer
	vertex_->Create(device, sizeof(VertexData2D) * 6);
	vertex_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	// view
	vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress(); // 先頭アドレスから使用
	vertexBufferView_.SizeInBytes = sizeof(VertexData2D) * 6; // 使用するサイズ（頂点6つ分）
	vertexBufferView_.StrideInBytes = sizeof(VertexData2D); // １頂点当たりのサイズ
	// Data書き込み(初期)
	VertexDataWrite();

	/// ===index=== ///
	// buffer
	index_->Create(device, sizeof(uint32_t) * 6);
	index_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	// view
	indexBufferView_.BufferLocation = index_->GetBuffer()->GetGPUVirtualAddress(); // 先頭のアドレスから使用
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6; // 使用するサイズ（６つ分）
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT; // uint32_tとする
	// Data書き込み(初期)
	IndexDataWrite();

	/// ===マテリアル=== ///
	// buffer
	material_->Create(device, sizeof(MaterialData2D) * 3);
	material_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// Data書き込み(初期)
	materialData_->color = color_;
	materialData_->uvTransform = MakeIdentity4x4();

	/// ===wvp=== ///
	// buffer
	wvp_->Create(device, sizeof(TransformationMatrix2D));
	wvp_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&wvpMatrixData_));
	// Data書き込み(初期)
	wvpMatrixData_->WVP = MakeIdentity4x4();

	/// ===Pipeline=== ///
	pipelineCommon_ = std::make_unique<PipelineStateObjectCommon>(); 
	pipelineCommon_->Create(PipelinType::Obj2D, mode);

	/// ===WorldTransformの設定=== ///
	worldTransform_ = { {1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, }, { 0.0f, 0.0f, 0.0f } };

	/// ===読み込んだテクスチャのサイズ道理にする=== ///
	AdjustTextureSize();
}


///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Sprite::Update() {

	// Data書き込み(更新)
	UpdateVertexDataWrite();
	SpecifyRange();
	TransformDataWrite();
	// カラー書き込み(更新)
	materialData_->color = color_;
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Sprite::Draw() {

	// Data書き込み(更新)
	UpdateVertexDataWrite();
	SpecifyRange();

	TransformDataWrite();
	// カラー書き込み(更新)
	materialData_->color = color_;

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12GraphicsCommandList* commandList = System::GetDXCommandList();

	/// ===コマンドリストに設定=== ///
	// PSOの設定
	pipelineCommon_->SetPSO(commandList);
	// プリミティブトポロジーをセット
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// VertexBufferViewの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IndexBufferViewの設定
	commandList->IASetIndexBuffer(&indexBufferView_);
	// Materialの設定
	commandList->SetGraphicsRootConstantBufferView(0, material_->GetBuffer()->GetGPUVirtualAddress());
	// wvpMatrixBufferの設定
	commandList->SetGraphicsRootConstantBufferView(1, wvp_->GetBuffer()->GetGPUVirtualAddress());
	// テクスチャの設定
	if (isLoadTexture_) {
		commandList->SetGraphicsRootDescriptorTable(2, System::GetSRVHandleGPU(textureIndex));
	}
	// 描画(ドローコール)
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}


///-------------------------------------------/// 
/// Resourceの作成関数
///-------------------------------------------///
ComPtr<ID3D12Resource> Sprite::CreateResource(ID3D12Device* device, size_t sizeInBytes) {
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
	ComPtr<ID3D12Resource> Resource = nullptr;
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));

	// 値を返す
	return Resource;
}


///-------------------------------------------/// 
/// VertexResourceの書き込み
///-------------------------------------------///
void Sprite::VertexDataWrite() {
	// 左下
	vertexData_[0].position = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertexData_[0].texcoord = { 0.0f, 1.0f };
	// 左上
	vertexData_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertexData_[1].texcoord = { 0.0f, 0.0f };
	// 右下
	vertexData_[2].position = { 1.0f, 1.0f, 0.0f, 1.0f };
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	// 右上
	vertexData_[3].position = { 1.0f, 0.0f, 0.0f, 1.0f };
	vertexData_[3].texcoord = { 1.0f, 0.0f };
}

///-------------------------------------------///  
/// UpdateVertexDataWrite
///-------------------------------------------///
void Sprite::UpdateVertexDataWrite() {

	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	// 左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}

	// 上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	// 左下
	vertexData_[0].position = { left, bottom, 0.0f, 1.0f };
	// 左上
	vertexData_[1].position = { left, top, 0.0f, 1.0f };
	// 右下
	vertexData_[2].position = { right, bottom, 0.0f, 1.0f };
	// 右上
	vertexData_[3].position = { right, top, 0.0f, 1.0f };
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

	// データの書き込み
	wvpMatrixData_->WVP = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
}


///-------------------------------------------/// 
/// テクスチャ範囲指定
///-------------------------------------------///
void Sprite::SpecifyRange() {
	const DirectX::TexMetadata& metadata =System::GetMetaData(textureIndex);
	float tex_left = textureLeftTop_.x / metadata.width;
	float tex_right = (textureLeftTop_.x + textureSize_.x) / metadata.width;
	float tex_top = textureLeftTop_.y / metadata.height;
	float tex_bottom = (textureLeftTop_.y + textureSize_.y) / metadata.height;

	// 頂点リソースにデータを書き込む
	vertexData_[0].texcoord = { tex_left, tex_bottom };
	vertexData_[1].texcoord = { tex_left, tex_top };
	vertexData_[2].texcoord = { tex_right, tex_bottom };
	vertexData_[3].texcoord = { tex_right, tex_top };
}


///-------------------------------------------/// 
/// テクスチャサイズをイメージに合わせる
///-------------------------------------------///
void Sprite::AdjustTextureSize() {
	// テクスチャメタデータを取得
	const DirectX::TexMetadata& metadata = System::GetMetaData(textureIndex);

	textureSize_.x = static_cast<float>(metadata.width);
	textureSize_.y = static_cast<float>(metadata.height);

	// 画像サイズをテクスチャサイズに合わせる
	size_ = textureSize_;
}
