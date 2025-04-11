#pragma once
#include "Engine/Graphics/OffScreen/RenderTarget.h"
#include <memory>

///=====================================================/// 
/// RenderPass 
///=====================================================///
class RenderPass {
public:
	RenderPass() = default;
	virtual ~RenderPass() = default;

	virtual void Initialize(std::shared_ptr<RenderTarget> renderTarget);
	virtual void Draw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) = 0;

protected:
	std::shared_ptr<RenderTarget> renderTarget_;
};

