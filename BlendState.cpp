#include "BlendState.h"

BlendState* BlendState::GetInstance() {
	static BlendState instance;
	return &instance;
}

/// <summary>
/// ブレンドステートの設定
/// </summary>
void BlendState::Setting() {
	// BlendStateの設定
	// すべての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

/// <summary>
/// ブレンドステートの取得
/// </summary>
D3D12_BLEND_DESC BlendState::GetBlendDesc() { return blendDesc_; }