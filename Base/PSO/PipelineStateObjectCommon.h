#pragma once
/// ===Include=== ///
// Engine
#include "Base//PSO/RootSignature/RootSignature.h"
#include "Base//PSO/InputLayout/InputLayout.h"
#include "Base//PSO/BlendState/BlendState.h"
#include "Base/PSO/DepthStencil/DepthStencil.h"
#include "Base//PSO/RasterizerState/RasterizerState.h"
#include "Base//PSO/Compiler/Compiler.h"

#include "Base/PSO/PipelineStateObjectType.h"

// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

// c++
#include <memory>
#include <array>
#include <cassert>

/// ===前方宣言=== ///
class DXCommon;

class PipelineStateObjectCommon {
public:

	PipelineStateObjectCommon();
	~PipelineStateObjectCommon();

	/// <summary>
	/// PSOの生成
	/// </summary>
	void Create(PipelinType Type, BlendMode Mode);

	/// <summary>
	/// PSOのSetter
	/// </summary>
	void SetPSO(ID3D12GraphicsCommandList* commandList);

private:

	std::unique_ptr<RootSignature> rootSignature_;     // RootSignature
	std::unique_ptr<InputLayout> inputLayout_;         // InputLayout
	std::unique_ptr<BlendState> blendState_;           // BlendState
	std::unique_ptr<RasterizerState> rasterizerState_; // RasterizerState
	std::unique_ptr<DepthStencil> depthStencil_;       // DepthStencil
	std::unique_ptr<Compiler> compiler_;               // Compiler

	ComPtr<ID3D12PipelineState> graphicsPipelineState_;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};

private:/// ====== ///

	/// <summary>
	/// DepthStencilDecの生成
	/// </summary>
	/// <returns></returns>
	D3D12_DEPTH_STENCIL_DESC CreateDepthStencilDesc();

	/// <summary>
	/// PipelineStateの生成
	/// </summary>
	void CreatePipelineState(DXCommon* dxCommon);
};