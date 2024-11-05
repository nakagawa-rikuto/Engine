#include "RasterizerState.h"

///-------------------------------------------/// 
/// ラスタライザステートの生成
///-------------------------------------------///
void RasterizerState::Create(PipelinType Type) {
	// RasterizerStateの設定
	if (Type == PipelinType::Obj2D) {

		// カリングしない（裏面も描画させる）
		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
	} else {

		// カリングしない（裏面も描画させる）
		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
	}
	
	// 三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
D3D12_RASTERIZER_DESC RasterizerState::GetRasterizerState()const { return rasterizerDesc_; }