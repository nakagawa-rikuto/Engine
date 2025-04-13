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
class DSVManager;

///=====================================================/// 
/// RenderTarget
///=====================================================///
class RenderTarget {
public:

	RenderTarget() = default;
	~RenderTarget() = default;

	// 初期化
	void Initialize(SRVManager* srv, RTVManager* rtv);

	// RenderTargetの生成
	void CreateRenderTexture(ID3D12Device* device);
	// RenderTargetのクリア
	void Clear(ID3D12GraphicsCommandList* commandList, DSVManager* dsv);

public:

	// RenderTargetの取得
	ID3D12Resource* GetBuffer();
	// RTVHandleの取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle();
	// SRVHandleの取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandle();
	// RTVのインデックス番号の取得
	uint32_t GetRTVHandleIndex() const;
	// SRVのインデックス番号の取得
	uint32_t GetSRVHandleIndex() const;
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
	uint32_t rtvHandleIndex_ = 0;
	uint32_t srvHandleIndex_ = 0;
};

