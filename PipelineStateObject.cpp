#include "PipelineStateObject.h"

#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <format>

PipelineStateObject* PipelineStateObject::GetInstance() {
	static PipelineStateObject instatnce;
	return &instatnce;
}

void PipelineStateObject::CreatePSO(DXCommon* dxCommon) {

	HRESULT hr;

	// RootSignatureの生成
	rootSignature_ = std::make_unique<RootSignature>();
	rootSignature_->Create(dxCommon->GetDevice());

	// InputLayoutの設定
	inputLayout_ = std::make_unique<InputLayout>();
	inputLayout_->Setting();

	// BlendStateの設定
	blendState_ = std::make_unique<BlendState>();
	blendState_->Setting();

	// RasterizerStateの設定
	rasterizerState_ = std::make_unique<RasterizerState>();
	rasterizerState_->Setting();

	// Compilerの初期化
	compiler_ = std::make_unique<Compiler>();
	compiler_->ShaderCompiler(dxCommon);

	// PSOの取得
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature_->GetRootSignature(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayout_->GetInputLayout(); // InputLayout
	graphicsPipelineStateDesc.VS = {
		compiler_->GetObjVS()->GetBufferPointer(), compiler_->GetObjVS()->GetBufferSize() }; // VertexShader
	graphicsPipelineStateDesc.PS = {
		compiler_->GetObjPS()->GetBufferPointer(), compiler_->GetObjPS()->GetBufferSize() }; // PixlShader
	graphicsPipelineStateDesc.BlendState = blendState_->GetBlendDesc(); // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerState_->GetRasterizerState(); // RasterizerState

	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 実際に生成
	hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// PSOの取得
/// </summary>
ID3D12PipelineState* PipelineStateObject::GetPSO() { return graphicsPipelineState_.Get(); }

/// <summary>
/// RootSignatureの取得
/// </summary>
ID3D12RootSignature* PipelineStateObject::GetRootSignature() {return rootSignature_->GetRootSignature();}