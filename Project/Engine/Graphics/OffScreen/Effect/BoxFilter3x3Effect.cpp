#include "BoxFilter3x3Effect.h"
// Service
#include "Engine/System/Service/Render.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void BoxFilter3x3Effect::Initialize(std::shared_ptr<RenderTexture> RenderTexture) {
	// RenderTextureを取得
	renderTexture_ = RenderTexture;
}

///-------------------------------------------/// 
/// 描画前処理
///-------------------------------------------///
void BoxFilter3x3Effect::PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {
	RenderPass::PreDraw(commandList, dsvHandle);
}

///-------------------------------------------/// 
/// ImGuiの描画
///-------------------------------------------///
void BoxFilter3x3Effect::Draw(ID3D12GraphicsCommandList* commandList) {
	// コピーイメージ
	Render::SetPSO(commandList, PipelineType::BoxFilter3x3, BlendMode::kBlendModeNone);

	commandList->SetGraphicsRootDescriptorTable(0, renderTexture_->GetSRVHandle());
	// 頂点3つを描画
	commandList->DrawInstanced(3, 1, 0, 0);
}