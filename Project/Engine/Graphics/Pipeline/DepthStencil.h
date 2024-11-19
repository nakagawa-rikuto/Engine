#pragma once
/// ===Include=== ///
// PipelineType
#include "Game/Data/PipelineStateObjectType.h"

// directX
#include <d3d12.h>

///=====================================================/// 
/// DepthStencil
///=====================================================///
class DepthStencil {
public:

	DepthStencil() = default;
	~DepthStencil() = default;

	/// <summary>
	/// デプスステンシルの生成
	/// </summary>
	void Create(PipelinType Type);

	/// <summary>
	/// デプスステンシルの取得
	/// </summary>
	/// <returns></returns>
	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() const;

private:

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{}; // DepthStencil
};