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
void DissolveEffect::Initialize(ID3D12Device* device, std::shared_ptr<RenderTexture> RenderTexture) {
	// RenderTextureを取得
	renderTexture_ = RenderTexture;

	// テクスチャ名を設定
	textureKeyName_ = "Dissolve";

	buffer_ = std::make_unique<BufferBase>();
	buffer_->Create(device, sizeof(DissolveData));
	buffer_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	// Dissolveエフェクトのデータを初期化
	data_->edgeColor = { 1.0f, 1.0f, 1.0f }; // エッジ色を白に設定
	data_->threshold = 0.5f; // デフォルトの閾値
	data_->edgeStart = 0.2f; // エッジの開始位置
	data_->edgeEnd = 0.8f; // エッジの終了位置
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
	// パイプラインの設定
	Render::SetPSO(commandList, PipelineType::Dissolve, BlendMode::kBlendModeNone);

	// dataの設定
	commandList->SetGraphicsRootConstantBufferView(2, buffer_->GetBuffer()->GetGPUVirtualAddress());

	// Textureの設定
	commandList->SetGraphicsRootDescriptorTable(0, renderTexture_->GetSRVHandle());
	Render::SetGraphicsRootDescriptorTable(commandList, 1, textureKeyName_);

	// 頂点3つを描画
	commandList->DrawInstanced(3, 1, 0, 0);
}

///-------------------------------------------/// 
/// ImGui情報
///-------------------------------------------///
void DissolveEffect::ImGuiInfo() {
	// ImGuiの描画
	ImGui::Text("Dissolve Effect");
	ImGui::SliderFloat("Threshold", &data_->threshold, 0.0f, 1.0f);
	ImGui::SliderFloat("Edge Start", &data_->edgeStart, 0.0f, 1.0f);
	ImGui::SliderFloat("Edge End", &data_->edgeEnd, 0.0f, 1.0f);
	ImGui::ColorEdit3("Edge Color", &data_->edgeColor.x);
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void DissolveEffect::SetDissolveData(float threshold, float edgeStart, float edgeEnd, const Vector3& edgeColor) {
	data_->threshold = threshold;
	data_->edgeStart = edgeStart;
	data_->edgeEnd = edgeEnd;
	data_->edgeColor = edgeColor;
}
