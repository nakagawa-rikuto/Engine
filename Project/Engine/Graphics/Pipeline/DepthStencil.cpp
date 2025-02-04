#include "DepthStencil.h"

///-------------------------------------------/// 
/// デプスステンシルの生成
///-------------------------------------------///
void DepthStencil::Create(PipelineType Type) {
	// DepthStencil
	// DepthStencil
	if (Type == PipelineType::ForGround2D) {

		// Depthの機能を有効化
		depthStencilDesc_.DepthEnable = true;

		// 書き込みします
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		// 比較関数はLessEqual。
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	} else if (Type == PipelineType::BackGround2D) {

		// Depthの機能を無効か
		depthStencilDesc_.DepthEnable = false;

		// 書き込み市内
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		// ｚ値に関係なく描画。
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

		// ステンシルバッファの無効化
		depthStencilDesc_.StencilEnable = false;
	} else if (Type == PipelineType::Obj3D) {

		// Depthの機能を有効化
		depthStencilDesc_.DepthEnable = true;

		// 書き込みします
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		// 比較関数はLessEqual。
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	} else if(Type == PipelineType::Particle){

		// Depthの機能を有効化
		depthStencilDesc_.DepthEnable = true;

		// 書き込みします
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

		// 比較関数はLessEqual。
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	} else {

		return;
	}
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
D3D12_DEPTH_STENCIL_DESC DepthStencil::GetDepthStencilDesc() const { return depthStencilDesc_; }