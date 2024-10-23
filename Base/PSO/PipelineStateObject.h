#pragma once
#include "Base//PSO/RootSignature/RootSignature.h"
#include "Base//PSO/InputLayout/InputLayout.h"
#include "Base//PSO/BlendState/BlendState.h"
#include "Base//PSO/RasterizerState/RasterizerState.h"
#include "Base//PSO/Compiler/Compiler.h"

/// ===前方宣言=== ///
class DXCommon;

class PipelineStateObject {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PipelineStateObject();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PipelineStateObject();

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

private:/// ====== ///

	/// <summary>
	/// DepthStencilDecの生成
	/// </summary>
	/// <returns></returns>
	D3D12_DEPTH_STENCIL_DESC CreateDepthStencilDesc();
};