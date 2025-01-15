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
	void VertexInitialize(ID3D12Device* device, uint32_t Size);
	void IndexInitialize(ID3D12Device* device, uint32_t Size);
	void MaterialInitialize(ID3D12Device* device, uint32_t Size);
	void WVPMatrixInitialize(ID3D12Device* device);

	/// <summary>
	/// 描画準備
	/// </summary>
	void Bind(ID3D12GraphicsCommandList* commandList, BlendMode mode);
public:

	// Dataの設定
	void SetData(VertexData2D* vertex, uint32_t* index, MaterialData2D* materal, TransformationMatrix2D* wvp);

	// Dataの設定
	void SetVertexData(const Vector4& position, const Vector2& texcoord);
	void SetIndexData(uint32_t& indexData);
	void SetMateialData(const Vector4& color, const Matrix4x4& uvTransform);
	void SetWVPData(const Matrix4x4& WVP);
	
private:

	// バッファリソース
	std::unique_ptr<VertexBuffer2D> vertex_;
	std::unique_ptr<IndexBuffer2D> index_;
	std::unique_ptr<Material2D> material_;
	std::unique_ptr<Transform2D> wvp_;

	// バッファリソース内のデータを指すポインタ
	VertexData2D* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	MaterialData2D* materialData_ = nullptr;
	TransformationMatrix2D* wvpMatrixData_ = nullptr;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
};

