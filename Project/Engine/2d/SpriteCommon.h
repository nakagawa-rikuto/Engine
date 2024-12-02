#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/CData.h"
#include "Engine/2d/VertexBuffer2D.h"
#include "Engine/2d/IndexBuffer2D.h"
#include "Engine/2d/Material2D.h"
#include "Engine/2d/Transform2D.h"
#include "Engine/Graphics/Pipeline/PipelineStateObjectType.h"
//c++
#include <memory>

///=====================================================/// 
/// スプライト共通部
///=====================================================///
class SpriteCommon {
public:

	SpriteCommon();
	~SpriteCommon();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void VertexInitialize(ID3D12Device* device, uint32_t Size, VertexData2D* data);
	void IndexInitialize(ID3D12Device* device, uint32_t Size, uint32_t* data);
	void MaterialInitialize(ID3D12Device* device, uint32_t Size, MaterialData2D* data);
	void WVPMatrixInitialize(ID3D12Device* device, TransformationMatrix2D* data);

	/// <summary>
	/// 描画準備
	/// </summary>
	void Bind(ID3D12GraphicsCommandList* commandList, BlendMode mode);

private:

	// バッファリソース
	std::unique_ptr<VertexBuffer2D> vertex_;
	std::unique_ptr<IndexBuffer2D> index_;
	std::unique_ptr<Material2D> material_;
	std::unique_ptr<Transform2D> wvp_;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
};

