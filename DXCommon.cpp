#include "DXCommon.h"
#include "Logger.h"

#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <format>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Winmm.lib")

namespace {
	const uint32_t kNumRTVDescriptor = 4;
}

/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
/// <returns></returns>
DXCommon* DXCommon::GetInstance() {
	static DXCommon instance;
	return &instance;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="winApp"></param>
/// <param name="backBufferWidth"></param>
/// <param name="backBufferHeight"></param>
void DXCommon::Initialize(
	WinApp* winApp, int32_t backBufferWidth, int32_t backBufferHeight) {
	// nullptrチェック
	assert(winApp);

	/* sleepの分解能を上げておく
	(システムタイマーのカウント数を1ミリ秒にする) */
	timeBeginPeriod(1);

	winApp_ = winApp;
	backBufferWidth_ = backBufferWidth;
	backBufferHeight_ = backBufferHeight;

	// DXGIデバイスの初期化
	InitializeDXGIDevice();

	// コマンド関連の初期化
	InitializeCommand();

	// スワップチェーンの生成
	CreateSwapChain();

	// レンダーターゲットの生成
	CreateFinalRenderTargets();

	// フェンスの生成
	CreateFence();

	// DXCの初期化
	InitializeCompiler();

	// ビューポート
	CreateViewport(backBufferWidth_, backBufferHeight_);

	// シザー矩形
	CreateScissor(backBufferWidth_, backBufferHeight_);
}

void DXCommon::PreDraw() {

	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	/* ///////////////////
		　 バリアを張る
	*/ ///////////////////
	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};

	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = swapChainResource_[backBufferIndex].Get();

	// 遷移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;

	// 遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	// TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier);

	// 描画先のRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, nullptr);

	// 全画面クリア
	ClearRenderTarget();

	commandList_->RSSetViewports(1, &viewPort_); // viewportを設定
	commandList_->RSSetScissorRects(1, &scissorRect_); // scissorRectを設定
}

void DXCommon::PostDraw() {
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	//コマンドリストの内容を確定させる。　すべてのコマンドを積んでからCloseすること
	hr = commandList_->Close();
	assert(SUCCEEDED(hr));

	/* ///////////////////
		コマンドをキックする
	*/ ///////////////////
	// GPUコマンドリストの実行を行わせる
	ID3D12CommandList* commandList[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandList);

	//GPUとOSに画面の交換を行うように通知する
	swapChain_->Present(1, 0);

	/* ///////////////////
		GPUにSignalを送る
	*/ ///////////////////
	// GPUがここまでたどり着いたときに、Fenceの当た値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_.Get(), ++fenceValue_);

	// Fenceの値が指定したSignal値にたどり着いているか確認する
	// GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence_->GetCompletedValue() < fenceValue_) {

		// FenceのSignalを待つためのイベントを作成する
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent != nullptr);

		// 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent);

		// イベント待つ
		WaitForSingleObject(fenceEvent, INFINITE);

		//イベントの解放
		CloseHandle(fenceEvent);
	}

	// 次のフレーム用のコマンドリストを準備
	hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

/// <summary>
/// レンダーターゲットのクリア
/// </summary>
void DXCommon::ClearRenderTarget() {
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f }; // 青色っぽい色。RGBAの順
	commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], clearColor, 0, nullptr);
}

/// <summary>
/// CPUのディスクリプターハンドルの取得 
/// </summary>
D3D12_CPU_DESCRIPTOR_HANDLE DXCommon::GetCPUDescriptorHandle(
	ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	// ディスクリプタの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);

	return handleCPU;
}

/// <summary>
/// GPUのディスクリプターハンドルの取得
/// </summary>
D3D12_GPU_DESCRIPTOR_HANDLE DXCommon::GetGPUDescriptorHandle(
	ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	// ディスクリプタの先頭を取得
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);

	return handleGPU;
}

/// <summary>
/// DXGIデバイス初期化
/// </summary>
void DXCommon::InitializeDXGIDevice() {

	/* /////////////////////////////////////////////////////
						DirectX12の初期化
	*/ /////////////////////////////////////////////////////
	// HEWSULTはWindows系のエラーコードであり、
	// 関数が成功したかどうかをSUCCEEDEDマクロで判断できる
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	// 初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、
	// どうにもできない場合が多いのでAssertにしておく
	assert(SUCCEEDED(hr));

	// 使用するアダプタ用の変数
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter = nullptr;

	//良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(
		1, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i) {

		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr)); // 取得できないのは一大事

		//  ソフトウェアアダプタでなければ採用！
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {

			// 採用したアダプタの情報をログに出力。wstringの方なので注意
			Log(std::format(L"Use Adapater:{}\n", adapterDesc.Description));
			break;
		}

		useAdapter = nullptr;
	}

	// 適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0
	};

	const char* featureLevelStrings[] = {
		"12.2", "12.1", "12.0"
	};

	//高い順に生成できるか試してみる
	for (size_t i = 0; i < _countof(levels); ++i) {

		//採用したアダプターでデバイスを生成
		hr = D3D12CreateDevice(useAdapter.Get(), levels[i], IID_PPV_ARGS(&device_));

		// 指定した機能レベルでデバイスが生成できたかを確認	
		if (SUCCEEDED(hr)) {
			//
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}

	//初期化完了のログを出す
	assert(device_ != nullptr);
	Log("Complete create D3D12Device!!!\n");

#ifdef _DEBUG
	/* /////////////////////////////////////////////////////
						エラー・警告、即ち停止
	*/ /////////////////////////////////////////////////////
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {

		// やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);

		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		// 警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		/* /////////////////////////////////////////////////////
							エラーと警告の抑制
		*/ /////////////////////////////////////////////////////
		// 抑制するメッセージ
		D3D12_MESSAGE_ID denyIds[] = {
			// Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			// https:/stackoverflow.com/questions/69805245/DirectX-12-application-is-crashing-in-windows11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		// 抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		// 指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
	}
#endif
}

/// <summary>
/// コマンド関連の初期化
/// </summary>
void DXCommon::InitializeCommand() {
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	// コマンドキューの生成
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = device_->CreateCommandQueue(
		&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(hr));

	// コマンドアロケータの生成
	hr = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(hr));

	// コマンドリストの生成
	hr = device_->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// スワップチェーンの生成
/// </summary>
void DXCommon::CreateSwapChain() {
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	// スワップチェーンを生成する
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = backBufferWidth_;      // 画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Height = backBufferHeight_;     // 画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色の形式
	swapChainDesc.SampleDesc.Count = 1;  // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2;  // ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;  // モニタに移したら、中身を破棄

	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), winApp_->GetHwnd(), &swapChainDesc,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));

	// スワップチェーンからResourceを引っ張ってくる
	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResource_[0]));

	// うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResource_[1]));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// レンダーターゲットの生成
/// </summary>
void DXCommon::CreateFinalRenderTargets() {
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	// RTVの生成(レンダーターゲットビュー)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー用
	heapDesc.NumDescriptors = kNumRTVDescriptor; // ダブルバッファ用に2つ以上
	hr = device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap_));
	// ディスクリプターヒープが作れなかったので起動できない
	assert(SUCCEEDED(hr));

	// RTVのサイズを取得
	const uint32_t rtvSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// RTVの設定(レンダーターゲットビュー)
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;  // 出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;  // 2dテクスチャとして書き込む

	// RTVを2つ作るのでディスクリプタを2つ用意
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

	// まず1つ目を作る。1つ目は最初のところに作る。作る場所をコリらで指定してあげる必要がある
	rtvHandles_[0] = GetCPUDescriptorHandle(rtvHeap_.Get(), rtvSize, 0);
	device_->CreateRenderTargetView(swapChainResource_[0].Get(), &rtvDesc, rtvHandles_[0]);

	//2つ目のディスクリプタハンドルを得る(自力で)
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//2つ目を作る
	device_->CreateRenderTargetView(swapChainResource_[1].Get(), &rtvDesc, rtvHandles_[1]);
}

/// <summary>
/// フェンスの生成
/// </summary>
void DXCommon::CreateFence() {
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	// 初期値0でFenceを作る
	hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// DXCの初期化
/// </summary>
void DXCommon::InitializeCompiler() {
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	// dxcUtils
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));

	// dxcCompiler
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	// IncludeHandler
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

/// <summary>
/// ビューポート
/// </summary>
void DXCommon::CreateViewport(const int32_t kClientWindth, const int32_t kClientHeight) {

	// クライアント領域のサイズと一緒にして画面全体に表示
	viewPort_.Width = float(kClientWindth);
	viewPort_.Height = float(kClientHeight);
	viewPort_.TopLeftX = 0;
	viewPort_.TopLeftY = 0;
	viewPort_.MinDepth = 0.0f;
	viewPort_.MaxDepth = 1.0f;
}

/// <summary>
/// シザー矩形
/// </summary>
void DXCommon::CreateScissor(const int32_t kClientWindth, const int32_t kClientHeight) {

	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = kClientWindth;
	scissorRect_.top = 0;
	scissorRect_.bottom = kClientHeight;
}

/// <summary>
/// DXGFactoryの取得
/// </summary>
IDXGIFactory7* DXCommon::GetDXGFactory() const { return dxgiFactory_.Get(); }

/// <summary>
/// デバイスの取得
/// </summary>
ID3D12Device* DXCommon::GetDevice() const { return device_.Get(); }

/// <summary>
/// dxcUtilsの取得
/// </summary>
IDxcUtils* DXCommon::GetDxcUtils() const { return dxcUtils_.Get(); }

/// <summary>
/// dxcCompilerの取得
/// </summary>
IDxcCompiler3* DXCommon::GetDxcCompiler() const { return dxcCompiler_.Get(); }

/// <summary>
/// IncludeHandlerの取得
/// </summary>
IDxcIncludeHandler* DXCommon::GetIncludeHandler() const { return includeHandler_.Get(); }

/// <summary>
/// 描画コマンドリストの取得
/// </summary>
ID3D12GraphicsCommandList* DXCommon::GetCommandList() const { return commandList_.Get(); }