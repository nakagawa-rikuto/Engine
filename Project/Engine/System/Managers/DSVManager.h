#pragma once
/// ===Include=== ///
// DirectX
#include <d3d12.h>
// c++
#include <cstdint>
#include <cassert>
// Engien
#include "Engine/Core/ComPtr.h"

/// ===前方宣言=== ///
class DXCommon;

///=====================================================/// 
/// DSVManager
///=====================================================///
class DSVManager {
public:

	DSVManager();
	~DSVManager();

	// 深度バッファの生成(DSV)
	void CreateDepthBuffer(DXCommon* dxcommon, ID3D12Resource* depthStencil);

public: /// ===Getter=== ///

	ID3D12DescriptorHeap* GetDescriptorHeap() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

private:

	// RTVの数
	static const uint32_t kNumDSVDescriptor_;
	// depthStencilResource
	ComPtr<ID3D12Resource> depthStencilResource_;
	// ヒープ
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandles_;
	// スクリプタサイズ
	uint32_t descriptorSize_ = 0;
};

