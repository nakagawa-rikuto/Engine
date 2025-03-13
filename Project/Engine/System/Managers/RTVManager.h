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
/// RTVManager
///=====================================================///
class RTVManager {
public:

	RTVManager();
	~RTVManager();

	// RTVの生成
	void CreateFinalRenderTargets(DXCommon* dxcommon, ID3D12Resource* backBuffers[], uint32_t backBufferCount);

public: /// ===Getter=== ///

	ID3D12DescriptorHeap* GetDescriptorHeap() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

private:

	// RTVの数
	static const uint32_t kNumRTVDescriptor_; 
	// ヒープ
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandles_[2];
	// スクリプタサイズ
	uint32_t descriptorSize_ = 0; 
};

