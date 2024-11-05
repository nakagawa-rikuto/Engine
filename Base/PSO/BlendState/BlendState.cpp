#include "BlendState.h"

///-------------------------------------------/// 
/// ブレンドステートの生成
///-------------------------------------------///
void BlendState::Create() {
	// BlendStateの設定
	// すべての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
D3D12_BLEND_DESC BlendState::GetBlendDesc() { return blendDesc_; }