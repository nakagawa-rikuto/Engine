#pragma once
/// ===Include=== ///
#include "Engine/Graphics/OffScreen/RenderPass.h"
// c++
#include <string>
// Math
#include "Math/Vector3.h"

///=====================================================/// 
/// CopyImageEffect
///=====================================================///
class DissolveEffect : public RenderPass {
public:
	DissolveEffect() = default;
	~DissolveEffect() = default;

	// 初期化
	void Initialize(std::shared_ptr<RenderTexture> RenderTexture) override;
	// 描画
	void PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) override;
	// 描画
	void Draw(ID3D12GraphicsCommandList* commandList) override;
	// ImGui情報
	void ImGuiInfo() override;

private:

	std::string textureKeyName_ = "Dissolve";

	// / Dissolveエフェクトのデータ構造
	struct DissolveData {
		float threshold; // discardの閾値
		float edgeStart; // smoothstep開始
		float edgeEnd; // smoothstep終了
		Vector3 edgeColor; // エッジ色
		float padding; // floatの倍数にするためのパディング
	};
	DissolveData dissolveData_;
};