#include "OffScreenRenderer.h"
// RenderPass
#include "SceneRenderPass.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
OffScreenRenderer::~OffScreenRenderer() {
	renderPass_.clear();
	renderTexture_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void OffScreenRenderer::Initialize(
	ID3D12Device* device,
	SRVManager* srv, RTVManager* rtv,
	uint32_t width, uint32_t height, const Vector4& clearColor) {

	// RenderTextureを初期化
	renderTexture_ = std::make_shared<RenderTexture>();
	renderTexture_->Initialize(srv, rtv, width, height, clearColor, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	renderTexture_->CreateRenderTexture(device);

	///-------------------------------------------/// 
	/// これからどんどん追加していく
	///-------------------------------------------///
	/// ===RenderPassの登録=== ///
	// SceneRenderPass
	std::shared_ptr<SceneRenderPass> scenePass = std::make_shared<SceneRenderPass>();
	scenePass->Initialize(renderTexture_);
	renderPass_.emplace_back(scenePass);
}

///-------------------------------------------/// 
/// 描画前処理
///-------------------------------------------///
void OffScreenRenderer::PreDraw(ID3D12GraphicsCommandList* commandList) {}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void OffScreenRenderer::Draw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {
	for (auto& pass : renderPass_) {
		pass->Draw(commandList, dsvHandle);
	}
}

///-------------------------------------------/// 
/// 描画後処理
///-------------------------------------------///
void OffScreenRenderer::PostDraw(ID3D12GraphicsCommandList* commandList) {}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// RTV
D3D12_CPU_DESCRIPTOR_HANDLE OffScreenRenderer::GetResultRTV() const { return renderTexture_->GetRTVHandle(); }
// SRV
D3D12_GPU_DESCRIPTOR_HANDLE OffScreenRenderer::GetResultSRV() const { return renderTexture_->GetSRVHandle(); }
// RTVIndex
uint32_t OffScreenRenderer::GetRTVHandleIndex() const { return renderTexture_->GetRTVHandleIndex(); }
// SRVIndex
uint32_t OffScreenRenderer::GetSRVHandleIndex() const { return renderTexture_->GetSRVHandleIndex(); }
