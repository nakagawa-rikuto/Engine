#include "SpriteCommon.h"

#include "Engine/Core/System.h"
#include "Math/sMath.h"

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
SpriteCommon::SpriteCommon() = default;
SpriteCommon::~SpriteCommon() {
	vertex_.reset();
	index_.reset();
	material_.reset();
	wvp_.reset();
}

///-------------------------------------------/// 
///
///-------------------------------------------///
void SpriteCommon::VertexInitialize(ID3D12Device* device, uint32_t size, VertexData2D* data) {
	// 生成
	vertex_ = std::make_unique<VertexBuffer2D>();
	// buffer
	vertex_->Create(device, sizeof(VertexData2D) * size);
	vertex_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&data));
	// view
	vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress(); // 先頭アドレスから使用
	vertexBufferView_.SizeInBytes = sizeof(VertexData2D) * size; // 使用するサイズ（頂点6つ分）
	vertexBufferView_.StrideInBytes = sizeof(VertexData2D); // １頂点当たりのサイズ
}

///-------------------------------------------/// 
///
///-------------------------------------------///
void SpriteCommon::IndexInitialize(ID3D12Device * device, uint32_t size, uint32_t * data) {
	// 生成
	index_ = std::make_unique<IndexBuffer2D>();
	// buffer
	index_->Create(device, sizeof(uint32_t) * size);
	index_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&data));
	// view
	indexBufferView_.BufferLocation = index_->GetBuffer()->GetGPUVirtualAddress(); // 先頭のアドレスから使用
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * size; // 使用するサイズ（６つ分）
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT; // uint32_tとする
}

///-------------------------------------------/// 
///
///-------------------------------------------///
void SpriteCommon::MaterialInitialize(ID3D12Device * device, uint32_t size, MaterialData2D * data) {
	// 生成
	material_ = std::make_unique<Material2D>();
	// buffer
	material_->Create(device, sizeof(MaterialData2D) * size);
	material_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&data));
}

///-------------------------------------------/// 
///
///-------------------------------------------///
void SpriteCommon::WVPMatrixInitialize(ID3D12Device * device, TransformationMatrix2D * data) {
	// 生成
	wvp_ = std::make_unique<Transform2D>();
	// buffer
	wvp_->Create(device, sizeof(TransformationMatrix2D));
	wvp_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&data));
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SpriteCommon::Bind(ID3D12GraphicsCommandList * commandList, BlendMode mode) {

	// PSOの設定
	System::SetPSO(commandList, PipelineType::Obj2D, mode);
	// VertexBufferViewの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IndexBufferViewの設定
	commandList->IASetIndexBuffer(&indexBufferView_);
	// Materialの設定
	commandList->SetGraphicsRootConstantBufferView(0, material_->GetBuffer()->GetGPUVirtualAddress());
	// wvpMatrixBufferの設定
	commandList->SetGraphicsRootConstantBufferView(1, wvp_->GetBuffer()->GetGPUVirtualAddress());
}