#include "PipelineStateObjectCommon.h"

#include <cassert>

#include "Base/DirectXCommon/DXCommon.h"

///-------------------------------------------/// 
/// コンストラクタ
///-------------------------------------------///
PipelineStateObjectCommon::PipelineStateObjectCommon() = default;

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
PipelineStateObjectCommon::~PipelineStateObjectCommon() {

	rootSignature_.reset();
	inputLayout_.reset();
	rasterizerState_.reset();
	compiler_.reset();
}

///-------------------------------------------/// 
/// PSOの作成
///-------------------------------------------///
void PipelineStateObjectCommon::Create(DXCommon* dxCommon, PipelinType Type) {

	// RootSignatureの生成
	rootSignature_ = std::make_unique<RootSignature>();
	rootSignature_->Create(dxCommon, Type);

	// InputLayoutの生成
	inputLayout_ = std::make_unique<InputLayout>();
	inputLayout_->Create(Type);

	// BlendStateの生成
	blendState_ = std::make_unique<BlendState>();
	blendState_->Create();

	// RasterizerStateの生成
	rasterizerState_ = std::make_unique<RasterizerState>();
	rasterizerState_->Create(Type);

	// Compilerの初期化
	compiler_ = std::make_unique<Compiler>();
	compiler_->Initialize(dxCommon, Type);

	// PipelineState
	CreatePipelineState(dxCommon);
}

/// <summary>
/// PSOの取得
/// </summary>
ID3D12PipelineState* PipelineStateObjectCommon::GetPSO() { return graphicsPipelineState_.Get(); }

/// <summary>
/// RootSignatureの取得
/// </summary>
ID3D12RootSignature* PipelineStateObjectCommon::GetRootSignature() {return rootSignature_->GetRootSignature();}

///-------------------------------------------/// 
/// DepthStencilDesc
///-------------------------------------------///
D3D12_DEPTH_STENCIL_DESC PipelineStateObjectCommon::CreateDepthStencilDesc() {

	// DepthStencilDescの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// Depthの機能を有効化
	depthStencilDesc.DepthEnable = true;

	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

	// 比較関数はLessEqual。
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	return depthStencilDesc;

}

void PipelineStateObjectCommon::CreatePipelineState(DXCommon* dxCommon) {

	HRESULT hr;

	// PSOの取得
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};
	graphicsPipelineStateDesc_.pRootSignature = rootSignature_->GetRootSignature(); // RootSignature
	graphicsPipelineStateDesc_.InputLayout = inputLayout_->GetInputLayout(); // InputLayout
	graphicsPipelineStateDesc_.VS = {
		compiler_->GetObjVS()->GetBufferPointer(), compiler_->GetObjVS()->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc_.PS = {
		compiler_->GetObjPS()->GetBufferPointer(), compiler_->GetObjPS()->GetBufferSize() }; // PixlShader
	graphicsPipelineStateDesc_.BlendState = blendState_->GetBlendDesc(); // BlendState
	graphicsPipelineStateDesc_.RasterizerState = rasterizerState_->GetRasterizerState(); // RasterizerState
	graphicsPipelineStateDesc_.DepthStencilState = CreateDepthStencilDesc();

	// 書き込むRTVの情報
	graphicsPipelineStateDesc_.NumRenderTargets = 1;
	graphicsPipelineStateDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 書き込むDSVの情報
	graphicsPipelineStateDesc_.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc_.SampleDesc.Count = 1;
	graphicsPipelineStateDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 実際に生成
	hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc_, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}
