#pragma once
/// ===Icnlde=== ///
// Buffer
#include "Engine/3d/VertexBuffer3D.h"
#include "Engine/3d/Material3D.h"
#include "ParticleSetUp.h"
// Pipleine
#include "Engine/Graphics/Pipeline/PipelineStateObjectType.h"
// c++
#include <memory>

///=====================================================/// 
/// Particle共通描画設定
///=====================================================///
class ParticleGroup {
public:

	ParticleGroup();
	~ParticleGroup();

	// 初期化
	void Initialze();
	// 更新
	void Update();
	// 描画
	void Darw(BlendMode mode = BlendMode::KBlendModeNormal);

public:

private:
	/// ===バッファリソース=== ///
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<Material3D> mateial_;
	std::unique_ptr<ParticleSetUp> instanceing_;

	/// ===バッファリソース内のデータを指すポインタ=== ///
	VertexData3D* vertexData_ = nullptr;

	/// ===モデルデータ=== ///
	ModelData modelData_;
	WorldTransform uvTransform_;

	/// ===バッファビュー=== ///
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	
};

