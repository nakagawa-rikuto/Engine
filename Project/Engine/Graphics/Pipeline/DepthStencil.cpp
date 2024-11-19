#include "DepthStencil.h"

///-------------------------------------------/// 
/// デプスステンシルの生成
///-------------------------------------------///
void DepthStencil::Create(PipelinType Type) {
	// DepthStencil
	if (Type == PipelinType::Obj2D) {

		// Depthの機能を有効化
		depthStencilDesc_.DepthEnable = true;

		// 書き込みします
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

		// 比較関数はLessEqual。
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	} else if (Type == PipelinType::Obj3D) {

		// Depthの機能を有効化
		depthStencilDesc_.DepthEnable = true;

		// 書き込みします
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

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