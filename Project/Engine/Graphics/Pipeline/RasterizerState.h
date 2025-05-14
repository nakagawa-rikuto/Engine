#pragma once
/// ===Include=== ///
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// directX
#include <d3d12.h>
#include <unordered_map>

///=====================================================/// 
/// RasterizerState
///=====================================================///
class RasterizerState {
public:

	RasterizerState() = default;
	~RasterizerState() = default;

	/// <summary>
	/// ラスタライザステートの設定
	/// </summary>
	/// <returns></returns>
	void Create(PipelineType Type);

	/// <summary>
	/// ラスタライザステートの取得
	/// </summary>
	/// <returns></returns>
	D3D12_RASTERIZER_DESC GetRasterizerState()const;

private:
	// RasterizerState
	D3D12_RASTERIZER_DESC rasterizerDesc_{}; 

	// 設定用テーブル
	static const std::unordered_map<PipelineType, D3D12_CULL_MODE> kCullModeTable_;
};
