#pragma once
/// ====Include== ///
// Engine
#include "Engine/Core/ComPtr.h"
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
// c++
#include <cstdlib>
#include <chrono>
#include <wrl.h>

#pragma comment(lib, "dxcompiler.lib")

/// ===前方宣言=== ///
class WinApp;

///=====================================================/// 
/// DirectX汎用
///=====================================================///
class DXCommon {
public:

	DXCommon() = default;
	~DXCommon();

	// 初期化
	void Initialize(
		WinApp* winApp, int32_t backBufferWidth, int32_t backBufferHeight);

	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();

	/// ===Heapの生成=== ///
	ComPtr<ID3D12DescriptorHeap> CreateRTVHeap(); // RTV
	ComPtr<ID3D12DescriptorHeap> CreateDSVHeap(); // DSV
	ComPtr<ID3D12DescriptorHeap> CreateSRVHeap(const uint32_t maxSrvCount); // SRV

	/// ===DescriptorSizeの取得=== ///
	const uint32_t GetRTVDescriptorSize(); // RTV
	const uint32_t GetDSVDescriptorSize(); // DSV
	const uint32_t GetSRVDescriptorSize(); // SRV

	/// ===クリア=== ///
	void ClearRenderTarget(); // レンダーターゲット
	void ClearDepthBuffer();  // 深度バッファ

public:/// ===Getter=== ///
	// DXGFactoryの取得
	IDXGIFactory7* GetDXGFactory()const;
	// デバイスの取得
	ID3D12Device* GetDevice()const;
	// DxcUtilsの取得
	IDxcUtils* GetDxcUtils()const;
	// DxcCompilerの取得
	IDxcCompiler3* GetDxcCompiler()const;
	// IncludeHandlerの取得
	IDxcIncludeHandler* GetIncludeHandler()const;
	// 描画コマンドリストの取得
	ID3D12GraphicsCommandList* GetCommandList()const;
	// swapChainの取得
	IDXGISwapChain4* GetSwapChain()const;
	// RTVの指定番号のCPUでスクリプタハンドルを取得する
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);
	// RTVの指定番号のGPUでスクリプタハンドルを取得する
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);
	// DSVの指定番号のCPUでスクリプタハンドルを取得する
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
	// DSVの指定番号のGPUでスクリプタハンドルを取得する
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);
	// バックバッファの横幅の取得
	int32_t GetBackBufferWidth()const;
	// バックバッファの縦幅の取得
	int32_t GetBackBufferHeight()const;
	// バックバッファの数を取得
	size_t GetBackBufferCount()const;
	// CPUのディスクリプターハンドルの取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(
		const ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	// GPUのディスクリプターハンドルの取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(
		const ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

public:/// ===定数=== ///

	// RTV
	static const uint32_t kNumRTVDescriptor; // RTVの数
	// DSV 
	static const uint32_t kNumDSVDescriptor; // DSVの数

private: // メンバ変数

	// ウィンドウズアプリケーション管理
	WinApp* winApp_;

	/* //////////////////
		　Direct3D関連
	*/ //////////////////
	ComPtr<IDXGIFactory7> dxgiFactory_; // DXGIFactory
	ComPtr<ID3D12Device> device_; // D3D12Device
	ComPtr<IDxcUtils> dxcUtils_; // DxcUtils
	ComPtr<IDxcCompiler3> dxcCompiler_; // DxcCompiler
	ComPtr<IDxcIncludeHandler> includeHandler_; // includeHandler

	/// ===command=== ///
	ComPtr<ID3D12GraphicsCommandList> commandList_; // CommandList
	ComPtr<ID3D12CommandAllocator> commandAllocator_; // CommandAllocator
	ComPtr<ID3D12CommandQueue> commandQueue_; // CommandQueue

	/// ===swapChain=== ///
	ComPtr<IDXGISwapChain4> swapChain_; // SwapChain
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	ComPtr<ID3D12Resource> swapChainResource_[2];

	/// ===backBuffer=== ///
	std::vector<ComPtr<ID3D12Resource>> backBuffers_; // BackBuffer

	/// ===RTV=== ///
	ComPtr<ID3D12DescriptorHeap> rtvHeap_; // レンダーターゲットビュー
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2]; // レンダーターゲットビューのディスクリプタ
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{}; // レンダーターゲットビューのデスク
	uint32_t descriptorSizeRTV_; // RTV(DescriptorSize)

	/// ===DSV=== ///
	ComPtr<ID3D12Resource> depthStencilResource_; // depthStencilResource
	ComPtr<ID3D12DescriptorHeap> dsvHeap_; // DSV
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc_{}; // DSVデスク
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_; // DSVハンドル	
	uint32_t descriptorSizeDSV_; // // DSV(DescriptorSize)

	/// ===fence=== ///
	ComPtr<ID3D12Fence> fence_; // Fence
	uint64_t fenceValue_ = 0;  // FenceValue

	/// ===バリア=== ///
	D3D12_RESOURCE_BARRIER barrier_{};

	/// ===FPS固定=== ///
	std::chrono::steady_clock::time_point reference_; // 記録時間(FPS固定用)

	/// ===viewPort=== ///
	D3D12_VIEWPORT viewPort_; // ビューポート

	/// ===scissor=== ///
	D3D12_RECT scissorRect_; // シザー矩形

	/// ===backBufferSize=== ///
	int32_t backBufferWidth_ = 0;
	int32_t backBufferHeight_ = 0;

private:/// ===関数=== ///

	// デバッグレイヤー
	void DebugLayer();
	// エラー・警告
	void DebugInfo();
	// DXGIデバイス初期化
	void InitializeDXGIDevice();
	// コマンド関連の初期化
	void InitializeCommand();
	// スワップチェーンの生成
	void CreateSwapChain();
	// ディスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	// レンダーターゲットの生成(RTV)
	void CreateFinalRenderTargets();
	// 深度バッファの生成(DSV)
	void CreateDepthBuffer();
	// フェンスの生成
	void CreateFence();
	// DXCの初期化
	void InitializeCompiler();
	// ビューポート
	void CreateViewport(const int32_t kClientWindth, const int32_t kClientHeight);
	// シザー矩形
	void CreateScissor(const int32_t kClientWindth, const int32_t kClientHeight);
	// FPS固定の初期化
	void InitializeFixFPS();
	// FPS固定の更新
	void UpdateFixFPS();
};
