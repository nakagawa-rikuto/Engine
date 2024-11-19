#pragma once
/// ===Include=== ///
#include "Base/ComPtr/ComPtr.h"

// directx
#include <d3d12.h>

// c++
#include <cstdint>
#include <cassert>

/// ===前方宣言=== ///
class DXCommon;

///=====================================================/// 
/// SRVManager
///=====================================================///
class SRVManager {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DXCommon* dxcommon);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 確保関数
	/// </summary>
	uint32_t Allocate();

	/// <summary>
	/// SRV生成（テクスチャ用）
	/// </summary>
	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);

	/// <summary>
	/// SRV生成（Structured Buffer用）
	/// </summary>
	void CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

public:/// ===Setter=== ///

	/// <summary>
	///	SRVセットコマンド
	/// </summary>
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

public:/// ===Getter=== ///

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

private:/// ===variable=== ///
	DXCommon* dXCommon_ = nullptr; // DXCommon

	// 最大SRV数（最大テクスチャ数）
	static const uint32_t kMaxSRVCount_;
	
	// SRV用のでスクリプタサイズ
	uint32_t descriptorSize_;

	// SRV用でスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	// 次に使用するSRVインデックス
	uint32_t useIndex = 0;
};

