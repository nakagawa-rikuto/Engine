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

	// 初期化
	void Initialize(DXCommon* dxcommon);
	// 深度バッファの生成(DSV)
	void CreateDepthBuffer();
	// クリア
	void ClearDepthBuffer(ID3D12GraphicsCommandList* commandList);

public: /// ===Getter=== ///
	// Handle
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle()const;
	// Heap
	ID3D12DescriptorHeap* GetDescriptorHeap() const;
	// CPU
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index) const;
	// GPU
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index) const;

public:/// ===定数=== ///

	// DSVの数
	static const uint32_t kNumDSVDescriptor_;

private:
	// DXCommon
	DXCommon* dxcommon_ = nullptr;
	// depthStencilResource
	ComPtr<ID3D12Resource> depthStencilResource_;
	// ヒープ
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandles_;
	// スクリプタサイズ
	uint32_t descriptorSize_ = 0;
};

