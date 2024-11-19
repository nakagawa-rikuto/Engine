#pragma once
/// ===Include=== ///
// PipelineType
#include "Game/Data/PipelineStateObjectType.h"

// directX
#include <d3d12.h>

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
	void Create(PipelinType Type);

	/// <summary>
	/// ラスタライザステートの取得
	/// </summary>
	/// <returns></returns>
	D3D12_RASTERIZER_DESC GetRasterizerState()const;

private:

	D3D12_RASTERIZER_DESC rasterizerDesc_{}; // RasterizerState
};
