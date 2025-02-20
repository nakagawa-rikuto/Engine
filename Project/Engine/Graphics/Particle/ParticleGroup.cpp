#include "ParticleGroup.h"
// Engine
#include "Engine/Core/Mii.h"
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
#include "Engine/System/Managers/SRVManager.h"
#include "Engine/System/Service/Getter.h"
// camera
#include "application/Drawing/3d/Camera.h"
// Math
#include "Math/sMath.h"
// c++
#include <cassert>
#include <fstream>

///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
ParticleGroup::ParticleGroup() {}
ParticleGroup::~ParticleGroup() { 
	vertex_.reset();
	setUp_.reset();
}



///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Texture
void ParticleGroup::SetTexture(const std::string& fileName) { modelData_.material.textureFilePath = fileName; }
// material
void ParticleGroup::SetMatiarlData(const Vector4& color, const Matrix4x4& uvTransform) { setUp_->SetMatiarlData(color, uvTransform); }
// instancing
void ParticleGroup::SetInstancingData(size_t index, const Vector4& color, const Matrix4x4& WVP, const Matrix4x4& World) { setUp_->SetInstancingData(index, color, WVP, World); }


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleGroup::Initialze(const std::string& filename, const uint32_t kNumMaxInstance) {

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12Device* device = Getter::GetDXDevice();

	// 引数の値を代入
	kNumMaxInstance_ = kNumMaxInstance;

	/// ===モデル読み込み=== ///
	modelData_ = Getter::GetModelData(filename); // ファイルパス

	/// ===生成=== ///
	vertex_ = std::make_unique<VertexBuffer3D>();
	setUp_ = std::make_unique<ParticleSetUp>();

	/// ===worldTransform=== ///
	worldTransform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
	cameraTransform_ = { {1.0f, 1.0f,1.0f}, {0.3f, 0.0f, 0.0f}, {0.0f, 4.0f, -10.0f} };
	uvTransform_ = { {1.0f, 1.0f,1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	/// ===vertex=== ///
	// Buffer
	vertex_->Create(device, sizeof(VertexData3D) * modelData_.vertices.size());
	vertex_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	// メモリコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData3D) * modelData_.vertices.size());
	// view
	vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * modelData_.vertices.size());
	vertexBufferView_.StrideInBytes = sizeof(VertexData3D);

	/// ===SetUp=== ///
	setUp_->Initlize(device, kNumMaxInstance_);

	/// ===SRV=== ///
	srvData_.srvManager_ = Mii::GetSRVManager(); // SRVManagerの取得
	// SRVを作成するDescriptorHeapの場所設定
	srvData_.srvIndex = srvData_.srvManager_->Allocate();
	srvData_.srvHandleCPU = srvData_.srvManager_->GetCPUDescriptorHandle(srvData_.srvIndex);
	srvData_.srvHandleGPU = srvData_.srvManager_->GetGPUDescriptorHandle(srvData_.srvIndex);
	// SRVの生成
	srvData_.srvManager_->CreateSRVForStructuredBuffer(
		srvData_.srvIndex, setUp_->GetInstancing(), kNumMaxInstance_, sizeof(ParticleForGPU));
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ParticleGroup::Darw(const uint32_t instance, BlendMode mode) {

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12GraphicsCommandList* commandList = Mii::GetDXCommandList();

	/// ===コマンドリストに設定=== ///
	// PSOの設定
	Mii::SetPSO(commandList, PipelineType::Particle, mode);
	// VertexBufferViewの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// materialの設定
	setUp_->Bind(commandList);
	// Instancingの設定
	commandList->SetGraphicsRootDescriptorTable(1, srvData_.srvHandleGPU);
	// テクスチャの設定
	Mii::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.material.textureFilePath);
	// 描画（Drawコール）
	commandList->DrawInstanced(UINT(modelData_.vertices.size()), instance, 0, 0);

}