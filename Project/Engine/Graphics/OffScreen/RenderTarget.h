#pragma once
// ComPtr
#include "Engine/Core/ComPtr.h"
// Vector4
#include "Math/Vector4.h"
// DIrectX
#include <d3d12.h>
// uint32_t
#include <cstdint>

/// ===前方宣言///
class SRVManager;
class RTVManager;

///=====================================================/// 
/// RenderTarget
///=====================================================///
class RenderTarget {
public:

	RenderTarget() = default;
	~RenderTarget() = default;

	// 初期化
	void Initialize(uint32_t width, uint32_t height, const Vector4& color);

	// RenderTargetの生成
	void CreateRenderTexture(ID3D12Device* device);
	// RTVの生成
	void CreateRTV();
	// SRVの生成
	void CreateSRV();

public:

	// RenderTargetの取得
	ID3D12Resource* GetBuffer();

private:
	// Manager
	RTVManager* rtvManager_ = nullptr;
	SRVManager* srvManager_ = nullptr;

	// Info
	uint32_t width_;
	uint32_t height_;
	DXGI_FORMAT format_;
	Vector4 clearColor_;

	// Resource
	ComPtr<ID3D12Resource> buffer_;
	// DescriptorHeap
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
};

