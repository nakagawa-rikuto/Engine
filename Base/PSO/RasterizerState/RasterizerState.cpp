#include "RasterizerState.h"

/// <summary>
/// シングルトンインスタンス
/// </summary>
/// <returns></returns>
RasterizerState* RasterizerState::GetInstance() {
	static RasterizerState instance;
	return &instance;
}

/// <summary>
/// ラスタライザステートの設定
/// </summary>
void RasterizerState::Setting() {
	// RasterizerStateの設定
	// 裏面(時計回り)を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;

	// 三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}

/// <summary>
/// ラスタライザステートの取得
/// </summary>
D3D12_RASTERIZER_DESC RasterizerState::GetRasterizerState()const { return rasterizerDesc_; }