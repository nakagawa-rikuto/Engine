#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/CData.h"
#include "Engine/Core/ComPtr.h"
#include "Engine/2d/VertexBuffer2D.h"
#include "Engine/2d/IndexBuffer2D.h"
#include "Engine/2d/Material2D.h"
#include "Engine/2d/Transform2D.h"
#include "Engine/Graphics/Pipeline/PipelineStateObjectCommon.h"
// Game
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
	void Initailze();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList, BlendMode mode);

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

	// テクスチャファイルパス
	std::string filePath_;                      

	// 読み込んでいるかのフラグ
	bool isLoadTexture_ = false;
};

