#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

#pragma comment(lib, "dxcompiler.lib")

/// ===前方宣言=== ///
class WinApp;

/// <summary>
/// DirectX汎用
/// </summary>
class DXCommon {
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static DXCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp"></param>
	/// <param name="backBufferWidth"></param>
	/// <param name="backBufferHeight"></param>
	void Initialize(
		WinApp* winApp, int32_t backBufferWidth, int32_t backBufferHeight);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
	void ClearDepthBuffer();

	/// <summary>
	/// DXGFactoryの取得
	/// </summary>
	/// <returns></returns>
	IDXGIFactory7* GetDXGFactory()const;

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDevice()const;

	/// <summary>
	/// DxcUtilsの取得
	/// </summary>
	/// <returns></returns>
	IDxcUtils* GetDxcUtils()const;

	/// <summary>
	/// DxcCompilerの取得
	/// </summary>
	/// <returns></returns>
	IDxcCompiler3* GetDxcCompiler()const;

	/// <summary>
	/// IncludeHandlerの取得
	/// </summary>
	/// <returns></returns>
	IDxcIncludeHandler* GetIncludeHandler()const;

	/// <summary>
	/// 描画コマンドリストの取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommandList()const;

	/// <summary>
	/// バックバッファの横幅の取得
	/// </summary>
	/// <returns></returns>
	int32_t GetBackBufferWidth()const;

	/// <summary>
	/// バックバッファの縦幅の取得
	/// </summary>
	/// <returns></returns>
	int32_t GetBackBufferHeight()const;

	/// <summary>
	/// バックバッファの数を取得
	/// </summary>
	/// <returns></returns>
	size_t GetBackBufferCount()const;

	/// <summary>
	/// SRVの指定番号のCPUでスクリプタハンドルを取得する
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// SRVの指定番号のGPUでスクリプタハンドルを取得する
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// RTVの指定番号のCPUでスクリプタハンドルを取得する
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// RTVの指定番号のGPUでスクリプタハンドルを取得する
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// DSVの指定番号のCPUでスクリプタハンドルを取得する
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// DSVの指定番号のGPUでスクリプタハンドルを取得する
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// レンダーターゲットをセットする
	/// </summary>
	/// <param name="sRGB"></param>
	//void SetRenderTargets(bool sRGB);

public:

	/// ===定数=== ///
	// SRV
	static const uint32_t kMaxSRVCount; // 最大SRV数（最大テクスチャ枚数）

private: // メンバ変数

	// ウィンドウズアプリケーション管理
	WinApp* winApp_;

	/* //////////////////
		　Direct3D関連
	*/ //////////////////
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_; // DXGIFactory
	Microsoft::WRL::ComPtr<ID3D12Device> device_; // D3D12Device

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_; // DxcUtils
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_; // DxcCompiler
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_; // includeHandler

	/// ===command=== ///
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_; // CommandList
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_; // CommandAllocator
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_; // CommandQueue

	/// ===swapChain=== ///
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_; // SwapChain
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResource_[2];

	/// ===backBuffer=== ///
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_; // BackBuffer

	/// ===RTV=== ///
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_; // レンダーターゲットビュー
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2]; // レンダーターゲットビューのディスクリプタ
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{}; // レンダーターゲットビューのデスク
	uint32_t descriptorSizeRTV_; // RTV(DescriptorSize)

	/// ===SRV=== ///
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_; // SRV(テクスチャ関連)
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_[99]{}; // SRVデスク
	uint32_t descriptorSizeSRV_; // SRV(DescriptorSize)

	/// ===DSV=== ///
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_; // depthStencilResource
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_; // DSV
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc_{}; // DSVデスク
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_; // DSVハンドル	
	uint32_t descriptorSizeDSV_; // // DSV(DescriptorSize)

	/// ===fence=== ///
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_; // Fence
	uint64_t fenceValue_ = 0;  // FenceValue

	/// ===FPS固定=== ///
	std::chrono::steady_clock::time_point reference_; // 記録時間(FPS固定用)

	/// ===viewPort=== ///
	D3D12_VIEWPORT viewPort_; // ビューポート

	/// ===scissor=== ///
	D3D12_RECT scissorRect_; // シザー矩形

	/// ===backBufferSize=== ///
	int32_t backBufferWidth_ = 0;
	int32_t backBufferHeight_ = 0;

private:

	/// <summary>
	/// DXGIデバイス初期化
	/// </summary>
	/// <param name="enableDebugLayer"></param>
	void InitializeDXGIDevice();

	/// <summary>
	/// コマンド関連の初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// レンダーターゲットの生成(RTV)
	/// </summary>
	void CreateFinalRenderTargets();

	/// <summary>
	/// 深度バッファの生成(DSV)
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// シェーダーリソースの生成(SRV)
	/// </summary>
	void CreateShaderResource();

	/// <summary>
	/// フェンスの生成
	/// </summary>
	void CreateFence();

	/// <summary>
	/// DXCの初期化
	/// </summary>
	void InitializeCompiler();

	/// <summary>
	/// ビューポート
	/// </summary>
	void CreateViewport(const int32_t kClientWindth, const int32_t kClientHeight);

	/// <summary>
	/// シザー矩形
	/// </summary>
	void CreateScissor(const int32_t kClientWindth, const int32_t kClientHeight);

	/// <summary>
	/// ImGuiの初期化
	/// </summary>
	void InitializeImGui();

	/// <summary>
	/// FPS固定の初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定の更新
	/// </summary>
	void UpdateFixFPS();

	/// <summary>
	/// CPUのディスクリプターハンドルの取得
	/// </summary>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// GPUのディスクリプターハンドルの取得
	/// </summary>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(
		const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
};
