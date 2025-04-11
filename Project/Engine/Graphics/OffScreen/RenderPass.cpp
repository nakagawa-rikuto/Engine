#include "RenderPass.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void RenderPass::Initialize(std::shared_ptr<RenderTarget> renderTarget) {
	renderTarget_ = renderTarget;
}
