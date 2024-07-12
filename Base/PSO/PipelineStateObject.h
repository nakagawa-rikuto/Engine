#pragma once
#include "DXCommon.h"
#include "RootSignature.h"
#include "InputLayout.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "Compiler.h"

class PipelineStateObject {

public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static PipelineStateObject* GetInstance();

	/// <summary>
	/// PSOの生成
	/// </summary>
	void CreatePSO(DXCommon* dxCommon);

	/// <summary>
	/// PSOを取得
	/// </summary>
	/// <returns></returns>
	ID3D12PipelineState* GetPSO();

	/// <summary>
	/// RootSignatureの取得
	/// </summary>
	/// <returns></returns>
	ID3D12RootSignature* GetRootSignature();

private:

	std::unique_ptr<RootSignature> rootSignature_;
	std::unique_ptr<InputLayout> inputLayout_;
	std::unique_ptr<BlendState> blendState_;
	std::unique_ptr<RasterizerState> rasterizerState_;
	std::unique_ptr<Compiler> compiler_;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
};