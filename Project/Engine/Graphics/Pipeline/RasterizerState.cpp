#include "RasterizerState.h"

///-------------------------------------------/// 
/// ラスタライザステートの生成
///-------------------------------------------///
void RasterizerState::Create(PipelineType Type) {
	// RasterizerStateの設定
	if (Type == PipelineType::Obj2D) {

		// カリングしない（裏面も描画させる）
		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
	} else if (Type == PipelineType::Obj3D) {

		// 裏面(時計回り)を表示しない
		rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	} else if (Type == PipelineType::Particle){

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