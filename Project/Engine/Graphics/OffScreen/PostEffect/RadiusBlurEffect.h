#pragma once
/// ===Include=== ///
#include "Engine/Graphics/OffScreen/RenderPass.h"

///=====================================================/// 
/// RadiusBlurEffect
///=====================================================///
class RadiusBlurEffect : RenderPass {
public:
	RadiusBlurEffect() = default;
	~RadiusBlurEffect() = default;
	// 初期化
	void Initialize(std::shared_ptr<RenderTexture> RenderTexture) override;
	// 描画
	void PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) override;
	// 描画
	void Draw(ID3D12GraphicsCommandList* commandList) override;
};

