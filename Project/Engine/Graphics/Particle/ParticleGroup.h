#pragma once
/// ===Icnlde=== ///
// Buffer
#include "Engine/Graphics/3d/VertexBuffer3D.h"
#include "ParticleSetUp.h"
// Pipleine
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <memory>

/// ===前方宣言=== ///
class Camera;
class SRVManager;

///=====================================================/// 
/// Particle共通描画設定
///=====================================================///
class ParticleGroup {
public:

	ParticleGroup();
	~ParticleGroup();

	// 初期化
	void Initialze(const std::string& filename, const uint32_t kNumMaxInstance);
	// 描画
	void Darw(const uint32_t instance, BlendMode mode);

public: /// ===Setter=== ///

	// Material
	void SetMatiarlData(const Vector4& color, const Matrix4x4& uvTransform);
	// Instancing
	void SetInstancingData(size_t index, const Vector4& color, const Matrix4x4& WVP, const Matrix4x4& World);

private:
	/// ===SRV=== ///
	struct ParticleSRV {
		SRVManager* srvManager_ = nullptr;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};
	ParticleSRV srvData_;

	/// ===バッファリソース=== ///
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<ParticleSetUp> setUp_;;

	/// ===バッファリソース内のデータを指すポインタ=== ///
	VertexData3D* vertexData_ = nullptr;

	/// ===バッファビュー=== ///
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	
	/// ===モデルデータ=== ///
	ModelData modelData_;
	WorldTransform uvTransform_;

	/// ===カメラ=== ///
	Camera* camera_ = nullptr;
	WorldTransform cameraTransform_;

	/// ===モデル情報=== ///
	uint32_t kNumMaxInstance_;
	WorldTransform worldTransform_;
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };


};

