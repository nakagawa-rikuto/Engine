#pragma once
/// ===Include=== ///
// c++
#include <vector>
// Offscreen
#include "RenderTexture.h"
#include "RenderPass.h"

///=====================================================/// 
/// OffScreenRenderer
///=====================================================///
class OffScreenRenderer {
public:
	OffScreenRenderer() = default;
	~OffScreenRenderer();

	// 初期化
	void Initialize(
		ID3D12Device* device, 
		SRVManager* srv, RTVManager* rtv, 
		uint32_t width, uint32_t height, const Vector4& clearColor);
	// 描画前処理
	void PreDraw(ID3D12GraphicsCommandList* commandList);
	// 描画処理
	void Draw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle);
	// 描画後処理
	void PostDraw(ID3D12GraphicsCommandList* commandList);

public: /// ===Getter=== ///
	// RTVHandleの取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetResultRTV() const;
	// SRVの取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetResultSRV() const;
	// RTVのインデックス番号の取得
	uint32_t GetRTVHandleIndex() const;
	// SRVのインデックス番号の取得
	uint32_t GetSRVHandleIndex() const;

private:
	std::shared_ptr<RenderTexture> renderTexture_;
	std::vector<std::shared_ptr<RenderPass>> renderPass_;
};

