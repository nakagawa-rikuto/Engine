#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Core/CData.h"
#include "Engine/3d/VertexBuffer3D.h"
#include "Engine/3d/Material3D.h"
#include "Engine/3d/Transform3D.h"
#include "Engine/Graphics/Light.h"
#include "Engine/Graphics/Pipeline/PipelineStateObjectCommon.h"
// Game
#include "Game/3d/Camera.h"
// c++
#include <memory>

///-------------------------------------------/// 
/// モデル共通部
///-------------------------------------------///
class ModelCommon {
public:

	ModelCommon() = default;
	~ModelCommon();

	// 初期化
	void VertexInitialize(ID3D12Device* device, ModelData& data);
	void MaterialInitialize(ID3D12Device* device);
	void WVPMatrixInitialize(ID3D12Device* device);
	void LightInitialize(ID3D12Device* device);
	// 描画準備
	void Bind(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode);

public:
	// Dataの設定
	void SetData(VertexData3D* vertex, MaterialData3D* material, TransformationMatrix3D* wvp, DirectionalLight* light);
private:

	// バッファリソース
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<Material3D> material_;
	std::unique_ptr<Transform3D> wvp_;
	std::unique_ptr<Light> light_;

	// バッファリソース内のデータを指すポインタ
	VertexData3D* vertexData_ = nullptr;
	MaterialData3D* materialData_ = nullptr;
	TransformationMatrix3D* wvpData_ = nullptr;
	DirectionalLight* lightData_ = nullptr;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
};

