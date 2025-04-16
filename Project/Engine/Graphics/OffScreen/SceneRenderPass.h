#pragma once
/// ===Include=== ///
#include "Engine/Graphics/OffScreen/RenderPass.h"

///=====================================================/// 
/// SceneRenderPass
///=====================================================///
class SceneRenderPass : public RenderPass {
public:
	SceneRenderPass() = default;
	~SceneRenderPass() = default;

	// 初期化
	void Initialize(std::shared_ptr<RenderTexture> RenderTexture) override;
	// 描画
	void Draw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) override;
};

