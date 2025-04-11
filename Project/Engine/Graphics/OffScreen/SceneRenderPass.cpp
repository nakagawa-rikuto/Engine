#include "SceneRenderPass.h"


///-------------------------------------------/// 
/// 初期化　
///-------------------------------------------///
void SceneRenderPass::Initialize(std::shared_ptr<RenderTarget> renderTarget) {
	// RenderTargetの初期化
	RenderPass::Initialize(renderTarget);
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SceneRenderPass::Draw(ID3D12GraphicsCommandList * commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {
	// 描画ターゲットの切り替え
	//commandList->OMSetRenderTargets(1, &renderTarget_->GetRTVHandle(), false, &dsvHandle);
}
