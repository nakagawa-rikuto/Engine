#include "SceneRenderPass.h"

///-------------------------------------------/// 
/// 初期化　
///-------------------------------------------///
void SceneRenderPass::Initialize(std::shared_ptr<RenderTexture> RenderTexture) {
	// RenderTextureを取得
	renderTexture_ = RenderTexture;
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SceneRenderPass::Draw(ID3D12GraphicsCommandList * commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandel = renderTexture_->GetRTVHandle();
	commandList->OMSetRenderTargets(1, &rtvHandel, false, &dsvHandle);

	// RenderTextureのクリア
	renderTexture_->Clear(commandList);
}
