#include "ParticleSetUp.h"
// c++
#include <cassert>
#include <fstream>
// Service
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/System/Service/Getter.h"
#include "Engine/System/Service/Render.h"
// Managers
#include "Engine/System/Managers/SRVManager.h"
// camera
#include "application/Drawing/3d/Camera.h"
// Math
#include "Math/sMath.h"


///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
ParticleSetUp::ParticleSetUp() {}
ParticleSetUp::~ParticleSetUp() { 
	vertex_.reset();
	common_.reset();
}


///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Texture
void ParticleSetUp::SetTexture(const std::string& fileName) { modelData_.material.textureFilePath = fileName; }
// material
void ParticleSetUp::SetMatiarlData(const Vector4& color, const Matrix4x4& uvTransform) { common_->SetMatiarlData(color, uvTransform); }
// instancing
void ParticleSetUp::SetInstancingData(size_t index, const Vector4& color, const Matrix4x4& WVP, const Matrix4x4& World) { common_->SetInstancingData(index, color, WVP, World); }


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleSetUp::Initialze(const std::string& filename, const uint32_t kNumMaxInstance) {

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12Device* device = Getter::GetDXDevice();

	// 引数の値を代入
	kNumMaxInstance_ = kNumMaxInstance;

	/// ===モデル読み込み=== ///
	modelData_ = Getter::GetModelData(filename); // ファイルパス

	/// ===生成=== ///
	vertex_ = std::make_unique<VertexBuffer3D>();
	common_ = std::make_unique<ParticleCommon>();

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
	common_->Initlize(device, kNumMaxInstance_);

	/// ===SRV=== ///
	srvData_.srvManager_ = ServiceLocator::GetSRVManager(); // SRVManagerの取得
	// SRVを作成するDescriptorHeapの場所設定
	srvData_.srvIndex = srvData_.srvManager_->Allocate();
	srvData_.srvHandleCPU = srvData_.srvManager_->GetCPUDescriptorHandle(srvData_.srvIndex);
	srvData_.srvHandleGPU = srvData_.srvManager_->GetGPUDescriptorHandle(srvData_.srvIndex);
	// SRVの生成
	srvData_.srvManager_->CreateSRVForStructuredBuffer(
		srvData_.srvIndex, common_->GetInstancing(), kNumMaxInstance_, sizeof(ParticleForGPU));


}


///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ParticleSetUp::Darw(const uint32_t instance, BlendMode mode) {

	/// ===コマンドリストのポインタの取得=== ///
	ID3D12GraphicsCommandList* commandList = Getter::GetDXCommandList();

	/// ===コマンドリストに設定=== ///
	// PSOの設定
	Render::SetPSO(commandList, PipelineType::Particle, mode);
	// VertexBufferViewの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// materialの設定
	common_->Bind(commandList);
	// Instancingの設定
	commandList->SetGraphicsRootDescriptorTable(1, srvData_.srvHandleGPU);
	// テクスチャの設定
	Render::SetGraphicsRootDescriptorTable(commandList, 2, modelData_.material.textureFilePath);
	// 描画（Drawコール）
	commandList->DrawInstanced(UINT(modelData_.vertices.size()), instance, 0, 0);

}