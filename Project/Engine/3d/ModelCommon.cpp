#include "ModelCommon.h"
// System
#include "Engine/Core/Mii.h"
// Math
#include "Math/sMath.h"
// c++
#include <cassert>
#include <fstream>

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ModelCommon::~ModelCommon() {
	vertex_.reset();
	material_.reset();
	wvp_.reset();
	light_.reset();
}

///-------------------------------------------/// 
/// Bufferの初期化
///-------------------------------------------///
// vertex
void ModelCommon::VertexInitialize(ID3D12Device* device, ModelData& data) {
	// 生成
	vertex_ = std::make_unique<VertexBuffer3D>();
	// 作成
	vertex_->Create(device, sizeof(VertexData3D)* data.vertices.size());
	vertex_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	// メモリーコピー
	std::memcpy(vertexData_, data.vertices.data(), sizeof(VertexData3D) * data.vertices.size());
	// view
	vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * data.vertices.size());
	vertexBufferView_.StrideInBytes = sizeof(VertexData3D);
}
// material
void ModelCommon::MaterialInitialize(ID3D12Device* device) {
	// 生成
	material_ = std::make_unique<Material3D>();
	// 作成
	material_->Create(device, sizeof(MaterialData3D));
	material_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// Dataの書き込み(初期)
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = 0;
	materialData_->uvTransform = MakeIdentity4x4();
}
// wvp
void ModelCommon::WVPMatrixInitialize(ID3D12Device* device) {
	// 生成
	wvp_ = std::make_unique<Transform3D>();
	// 生成
	wvp_->Create(device, sizeof(TransformationMatrix3D));
	wvp_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	// Dataの書き込み(初期)
	wvpData_->WVP = MakeIdentity4x4();
	wvpData_->World = MakeIdentity4x4();
}
// Light
void ModelCommon::LightInitialize(ID3D12Device* device) {
	// 生成
	light_ = std::make_unique<Light>();
	// 作成
	light_->Create(device, sizeof(DirectionalLight));
	light_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
	// Dataの書き込み(初期)
	lightData_->color = {1.0f, 1.0f, 1.0f, 1.0f};
	lightData_->direction = { 0.0f, 0.0f, 0.0f };
	lightData_->intensity = 1.0f;
}

///-------------------------------------------///  
/// 描画準備
///-------------------------------------------///
void ModelCommon::Bind(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode) {
	/// ===コマンドリストに設定=== ///
	// PSOの設定
	Mii::SetPSO(commandList, type, mode);
	// VertexBufferViewの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// Materialの設定
	commandList->SetGraphicsRootConstantBufferView(0, material_->GetBuffer()->GetGPUVirtualAddress());
	// wvpの設定
	commandList->SetGraphicsRootConstantBufferView(1, wvp_->GetBuffer()->GetGPUVirtualAddress());
	// Lightの設定
	commandList->SetGraphicsRootConstantBufferView(3, light_->GetBuffer()->GetGPUVirtualAddress());
}

///-------------------------------------------/// 
/// セットデータ
///-------------------------------------------///
void ModelCommon::SetData(
	VertexData3D* vertex, MaterialData3D* material, TransformationMatrix3D* wvp, DirectionalLight* light) {
	// VertexDataのセット
	vertexData_ = vertex;
	// MaterialDataのセット
	materialData_ = material;
	// wvpDataのセット
	wvpData_ = wvp;
	// LightDataのセット
	lightData_ = light;
}
