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

	// 初期化
	void Initialize(DXCommon* dxcommon);
	// クリア
	void ClearRenderTarget(ID3D12GraphicsCommandList* commandList);

public: /// ===Getter=== ///
	// Handle
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(uint32_t index)const;
	// Heap
	ID3D12DescriptorHeap* GetDescriptorHeap() const;
	// CPU
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index) const;
	// GPU
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index) const;

public:/// ===定数=== ///

	// RTVの数
	static const uint32_t kNumRTVDescriptor_;

private:
	// DXCommon
	DXCommon* dxcommon_ = nullptr;
	// ヒープ
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandles_[2];
	// スクリプタサイズ
	uint32_t descriptorSize_ = 0; 

private:

	// RTVの生成
	void CreateFinalRenderTargets();
};

