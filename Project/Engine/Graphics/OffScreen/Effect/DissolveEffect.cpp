#include "DissolveEffect.h"
// Service
#include "Engine/System/Service/Render.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif

///-------------------------------------------/// 
/// 初期化　
///-------------------------------------------///
void DissolveEffect::Initialize(std::shared_ptr<RenderTexture> RenderTexture) {
	// RenderTextureを取得
	renderTexture_ = RenderTexture;

	// テクスチャ名を設定
	textureKeyName_ = "Dissolve";

	dissolveData_ = {
		.threshold = 0.5f, // discardの閾値
		.edgeStart = 0.2f, // smoothstep開始
		.edgeEnd = 0.3f, // smoothstep終了
		.edgeColor = { 1.0f, 1.0f, 1.0f }, // エッジ色
		.padding = 0.0f // floatの倍数にするためのパディング
	};
}

///-------------------------------------------/// 
/// 描画前処理
///-------------------------------------------///
void DissolveEffect::PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {
	RenderPass::PreDraw(commandList, dsvHandle);
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void DissolveEffect::Draw(ID3D12GraphicsCommandList* commandList) {
	// コピーイメージ
	Render::SetPSO(commandList, PipelineType::Dissolve, BlendMode::kBlendModeNone);

	commandList->SetGraphicsRootDescriptorTable(0, renderTexture_->GetSRVHandle());
	Render::SetGraphicsRootDescriptorTable(commandList, 1, textureKeyName_);
	// Dissolveエフェクトのデータを設定
	commandList->SetGraphicsRoot32BitConstants(2, sizeof(DissolveData) / sizeof(float), &dissolveData_, 0);

	// 頂点3つを描画
	commandList->DrawInstanced(3, 1, 0, 0);
}

///-------------------------------------------/// 
/// ImGui情報
///-------------------------------------------///
void DissolveEffect::ImGuiInfo() {
	// ImGuiの描画
	ImGui::Text("Dissolve Effect");
	ImGui::SliderFloat("Threshold", &dissolveData_.threshold, 0.0f, 1.0f);
	ImGui::SliderFloat("Edge Start", &dissolveData_.edgeStart, 0.0f, 1.0f);
	ImGui::SliderFloat("Edge End", &dissolveData_.edgeEnd, 0.0f, 1.0f);
	ImGui::ColorEdit3("Edge Color", &dissolveData_.edgeColor.x);
}
