#include "DXCommon.h"
#include "Base/Log/Logger.h"
#include "Base/WinApp/WinApp.h"

#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <format>

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Winmm.lib")

/// ===定数=== ///
const uint32_t DXCommon::kNumRTVDescriptor = 2;
const uint32_t DXCommon::kNumDSVDescriptor = 1;
const uint32_t DXCommon::kMaxSRVCount = 512;

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void DXCommon::Initialize(
	WinApp* winApp, int32_t backBufferWidth, int32_t backBufferHeight) {
	// nullptrチェック
	assert(winApp);

	// FPS固定の初期化
	InitializeFixFPS();

	/* sleepの分解能を上げておく
	(システムタイマーのカウント数を1ミリ秒にする) */
	timeBeginPeriod(1);

	winApp_ = winApp;
	backBufferWidth_ = backBufferWidth;
	backBufferHeight_ = backBufferHeight;

	// デバッグレイヤー
	DebugLayer();

	// DXGIデバイスの初期化
	InitializeDXGIDevice();

	// エラー・警告
	DebugInfo();

	// コマンド関連の初期化
	InitializeCommand();

	// スワップチェーンの生成
	CreateSwapChain();

	// レンダーターゲットの生成
	CreateFinalRenderTargets();

	// 深度バッファの生成
	CreateDepthBuffer();

	// シェーダリソースの生成
	CreateShaderResource();

	// フェンスの生成
	CreateFence();

	// ビューポート
	CreateViewport(backBufferWidth_, backBufferHeight_);

	// シザー矩形
	CreateScissor(backBufferWidth_, backBufferHeight_);

	// DXCの初期化
	InitializeCompiler();

	// ImGuiの初期化
	InitializeImGui();
}

///-------------------------------------------/// 
/// 描画前処理
///-------------------------------------------///
void DXCommon::PreDraw() {

	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	/* ///////////////////
		　 バリアを張る
	*/ ///////////////////
	// TransitionBarrierの設定
	// 今回のバリアはTransition
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	// Noneにしておく
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier_.Transition.pResource = swapChainResource_[backBufferIndex].Get();

	// 遷移前(現在)のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;

	// 遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	// TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);

	// 描画先のRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, &dsvHandle_);

	// 描画先のRTVとDSVを設定する

	// 全画面クリア
	ClearRenderTarget();
	ClearDepthBuffer();

	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvHeap_.Get() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);

	// コマンドを積む
	commandList_->RSSetViewports(1, &viewPort_); // viewportを設定
	commandList_->RSSetScissorRects(1, &scissorRect_); // scissorを設定
}

///-------------------------------------------/// 
/// 描画後処理
///-------------------------------------------///
void DXCommon::PostDraw() {
	HRESULT hr;

	// ImGuiの内部コマンドを生成する
	ImGui::Render();

	// 実際のCommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList_.Get());

	// RenderTargetからPresentにする
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	// TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);

	// コマンドリストの内容を確定させる。
   // すべてのコマンドを積んでからCloseすること
	hr = commandList_->Close();
	assert(SUCCEEDED(hr));

	// GPUコマンドリストの実行を行わせる
	ID3D12CommandList* commandList[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandList); // コマンドリストをキック

	//GPUとOSに画面の交換を行うように通知する
	swapChain_->Present(1, 0); // スワップチェーンのバッファを表示

	// GPUがここまでたどり着いたときに、Fenceの当た値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_.Get(), ++fenceValue_); // フェンスを更新

	// Fenceの値が指定したSignal値にたどり着いているか確認する
	// GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence_->GetCompletedValue() < fenceValue_) {

		// FenceのSignalを待つためのイベントを作成する
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent != nullptr); // イベントの作成が成功したか確認

		// 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent); // フェンスのシグナル設定

		// イベント待つ
		WaitForSingleObject(fenceEvent, INFINITE); // 指定のイベントがシグナル状態になるまで待機

		//イベントの解放
		CloseHandle(fenceEvent); // イベントハンドルの解放
	}

	// FPS固定
	UpdateFixFPS(); // フレームレートを固定する処理

	// 次のフレーム用のコマンドリストを準備
	hr = commandAllocator_->Reset(); // コマンドアロケータをリセット
	assert(SUCCEEDED(hr)); // リセットが成功したか確認
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr); // 新しいコマンドリストをリセット
	assert(SUCCEEDED(hr)); // リセットが成功したか確認
}

///-------------------------------------------/// 
/// DescriptorHeapの生成
///-------------------------------------------///
ComPtr<ID3D12DescriptorHeap> DXCommon::CreateRTVHeap() {return CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kNumRTVDescriptor, false);}
ComPtr<ID3D12DescriptorHeap> DXCommon::CreateDSVHeap() {return CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kNumDSVDescriptor, false);}
ComPtr<ID3D12DescriptorHeap> DXCommon::CreateSRVHeap() {return CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);}

///-------------------------------------------/// 
/// DescriptorSizeの取得
///-------------------------------------------///
const uint32_t DXCommon::GetRTVDescriptorSize() { return device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV); }
const uint32_t DXCommon::GetDSVDescriptorSize() { return device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV); }
const uint32_t DXCommon::GetSRVDescriptorSize() { return device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV); }


///-------------------------------------------/// 
/// レンダーターゲットのクリア
///-------------------------------------------///
void DXCommon::ClearRenderTarget() {
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f }; // 青色っぽい色。RGBAの順
	commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], clearColor, 0, nullptr);
}

///-------------------------------------------/// 
/// 深度バッファのクリア
///-------------------------------------------///
void DXCommon::ClearDepthBuffer() {

	commandList_->ClearDepthStencilView(dsvHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

///-------------------------------------------/// 
/// 各でスクリプタハンドルの取得用関数
///-------------------------------------------///
// SRVの指定番号のCPUでスクリプタハンドルを取得する
D3D12_CPU_DESCRIPTOR_HANDLE DXCommon::GetSRVCPUDescriptorHandle(uint32_t index) { return GetCPUDescriptorHandle(srvHeap_, descriptorSizeSRV_, index); }

// SRVの指定番号のGPUでスクリプタハンドルを取得する
D3D12_GPU_DESCRIPTOR_HANDLE DXCommon::GetSRVGPUDescriptorHandle(uint32_t index) { return GetGPUDescriptorHandle(srvHeap_, descriptorSizeSRV_, index); }

// RTVの指定番号のCPUでスクリプタハンドルを取得する
D3D12_CPU_DESCRIPTOR_HANDLE DXCommon::GetRTVCPUDescriptorHandle(uint32_t index) { return GetCPUDescriptorHandle(rtvHeap_, descriptorSizeRTV_, index); }

// RTVの指定番号のGPUでスクリプタハンドルを取得する
D3D12_GPU_DESCRIPTOR_HANDLE DXCommon::GetRTVGPUDescriptorHandle(uint32_t index) { return GetGPUDescriptorHandle(rtvHeap_, descriptorSizeRTV_, index); }

// DSVの指定番号のCPUでスクリプタハンドルを取得する
D3D12_CPU_DESCRIPTOR_HANDLE DXCommon::GetDSVCPUDescriptorHandle(uint32_t index) { return GetCPUDescriptorHandle(dsvHeap_, descriptorSizeDSV_, index); }

// DSVの指定番号のGPUでスクリプタハンドルを取得する
D3D12_GPU_DESCRIPTOR_HANDLE DXCommon::GetDSVGPUDescriptorHandle(uint32_t index) { return GetGPUDescriptorHandle(dsvHeap_, descriptorSizeDSV_, index); }

///-------------------------------------------/// 
/// デバッグレイヤー
///-------------------------------------------///
void DXCommon::DebugLayer() {
	ComPtr<ID3D12Debug1> debugController = nullptr;
	if ((SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))) {

		// デバッグレイヤーを有効化
		debugController->EnableDebugLayer();

		// さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
}

///-------------------------------------------/// 
/// エラー・警告
///-------------------------------------------///
void DXCommon::DebugInfo() {
	ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {

		// やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);

		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		// 警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		// 解放
		//infoQueue->Release();

		/// *****************************************************
		/// エラーと警告の抑制
		/// *****************************************************
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
}

///-------------------------------------------/// 
/// DXGIデバイス初期化
///-------------------------------------------///
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
	ComPtr<IDXGIAdapter4> useAdapter = nullptr;

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
	ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
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

///-------------------------------------------/// 
/// コマンド関連の初期化
///-------------------------------------------///
void DXCommon::InitializeCommand() {
	HRESULT hr;

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

///-------------------------------------------/// 
/// スワップチェーンの生成
///-------------------------------------------///
void DXCommon::CreateSwapChain() {
	HRESULT hr;

	// スワップチェーンを生成する
	swapChainDesc_.Width = backBufferWidth_;      // 画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Height = backBufferHeight_;     // 画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色の形式
	swapChainDesc_.SampleDesc.Count = 1;  // マルチサンプルしない
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 描画のターゲットとして利用する
	swapChainDesc_.BufferCount = 2;  // ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;  // モニタに移したら、中身を破棄

	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), winApp_->GetHwnd(), &swapChainDesc_,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));

	// スワップチェーンからResourceを引っ張ってくる
	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResource_[0]));

	// うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResource_[1]));
	assert(SUCCEEDED(hr));
}

///-------------------------------------------/// 
/// ディスクリプタヒープの生成
///-------------------------------------------///
ComPtr<ID3D12DescriptorHeap> DXCommon::CreateDescriptorHeap(
	D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {
	ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}

///-------------------------------------------/// 
/// レンダーターゲットの生成(RTV)
///-------------------------------------------///
void DXCommon::CreateFinalRenderTargets() {
	HRESULT hr;

	// DescriptorSizeを取得,Heapの生成
	rtvHeap_ = CreateRTVHeap();
	descriptorSizeRTV_ = GetRTVDescriptorSize();
	
	// RTVの生成(レンダーターゲットビュー)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー用
	heapDesc.NumDescriptors = kNumRTVDescriptor; // ダブルバッファ用に2つ以上
	heapDesc.Flags = false ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap_));
	// ディスクリプターヒープが作れなかったので起動できない
	assert(SUCCEEDED(hr));

	// RTVのサイズを取得
	const uint32_t rtvSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// RTVの設定(レンダーターゲットビュー)
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;  // 出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;  // 2dテクスチャとして書き込む

	// まず1つ目を作る。1つ目は最初のところに作る。作る場所をコリらで指定してあげる必要がある
	rtvHandles_[0] = GetCPUDescriptorHandle(rtvHeap_.Get(), rtvSize, 0);
	device_->CreateRenderTargetView(swapChainResource_[0].Get(), &rtvDesc_, rtvHandles_[0]);

	//2つ目のディスクリプタハンドルを得る
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//2つ目を作る
	device_->CreateRenderTargetView(swapChainResource_[1].Get(), &rtvDesc_, rtvHandles_[1]);
}

///-------------------------------------------/// 
/// 深度バッファの生成(DSV)
///-------------------------------------------///
void DXCommon::CreateDepthBuffer() {
	HRESULT hr;

	// DescriptorSizeの取得, Heapの生成
	dsvHeap_ = CreateDSVHeap();
	descriptorSizeDSV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = winApp_->kWindowWidth; // Textureの幅
	resourceDesc.Height = winApp_->kWindowHeight; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行 or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。

	// DepthStencilTextureをウィンドウのサイズで作成
	hr = device_->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // HEapの特殊な設定
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&depthStencilResource_)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture

	// DSVHeapの先頭にDSVをつくる
	device_->CreateDepthStencilView(
		depthStencilResource_.Get(), &dsvDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());

	// 描画先のRTVとDSVを設定する
	dsvHandle_ = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
}

///-------------------------------------------/// 
/// シェーダーリソースの生成(SRV)
///-------------------------------------------///
void DXCommon::CreateShaderResource() {

	srvHeap_ = CreateSRVHeap();
	descriptorSizeSRV_ = GetSRVDescriptorSize();
}

///-------------------------------------------/// 
/// フェンスの生成
///-------------------------------------------///
void DXCommon::CreateFence() {
	HRESULT hr;

	// 初期値0でFenceを作る
	hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));
}

///-------------------------------------------/// 
/// DXCの初期化
///-------------------------------------------///
void DXCommon::InitializeCompiler() {
	HRESULT hr;

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

///-------------------------------------------/// 
/// ビューポート
///-------------------------------------------///
void DXCommon::CreateViewport(const int32_t kClientWindth, const int32_t kClientHeight) {

	// クライアント領域のサイズと一緒にして画面全体に表示
	viewPort_.Width = float(kClientWindth);
	viewPort_.Height = float(kClientHeight);
	viewPort_.TopLeftX = 0;
	viewPort_.TopLeftY = 0;
	viewPort_.MinDepth = 0.0f;
	viewPort_.MaxDepth = 1.0f;
}

///-------------------------------------------/// 
/// シザー矩形
///-------------------------------------------///
void DXCommon::CreateScissor(const int32_t kClientWindth, const int32_t kClientHeight) {

	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = kClientWindth;
	scissorRect_.top = 0;
	scissorRect_.bottom = kClientHeight;
}

///-------------------------------------------/// 
/// ImGuiの初期化
///-------------------------------------------///
void DXCommon::InitializeImGui() {

	IMGUI_CHECKVERSION(); //　バージョンチェック
	ImGui::CreateContext(); // コンテキストの生成
	ImGui::StyleColorsDark(); // スタイルの設定
	ImGui_ImplWin32_Init(winApp_->GetHwnd()); // Win32用の初期化 
	ImGui_ImplDX12_Init( // DirectX12用の初期化
		device_.Get(),
		swapChainDesc_.BufferCount,
		rtvDesc_.Format,
		srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart());
}

///-------------------------------------------/// 
/// ImGuiの開始処理
///-------------------------------------------///
void DXCommon::BeginImGui() {
	// フレームの先頭でImGuiに、ここからフレームが始まる旨を告げる
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の初期に置き換える
	//ImGui::ShowDemoWindow();
}

///-------------------------------------------/// 
/// ImGuiの終了処理
///-------------------------------------------///
void DXCommon::EndImGui() {
	// ImGuiの終了処理.。
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

///-------------------------------------------/// 
/// FPS固定の初期化
///-------------------------------------------///
void DXCommon::InitializeFixFPS() {

	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

///-------------------------------------------/// 
/// FPS固定の更新
///-------------------------------------------///
void DXCommon::UpdateFixFPS() {

	// 1/60秒ピッタリの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));

	// 1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	// 現在の時間を取得
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// 前回の記録からの経過時間を取得する
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒(よりわずかに短い時間)経っていない場合
	if (elapsed < kMinTime) {
		// 1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			// 1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// DXGFactoryの取得
IDXGIFactory7* DXCommon::GetDXGFactory() const { return dxgiFactory_.Get(); }

// デバイスの取得
ID3D12Device* DXCommon::GetDevice() const { return device_.Get(); }

// dxcUtilsの取得
IDxcUtils* DXCommon::GetDxcUtils() const { return dxcUtils_.Get(); }

//  dxcCompilerの取得
IDxcCompiler3* DXCommon::GetDxcCompiler() const { return dxcCompiler_.Get(); }

// IncludeHandlerの取得
IDxcIncludeHandler* DXCommon::GetIncludeHandler() const { return includeHandler_.Get(); }

// 描画コマンドリストの取得
ID3D12GraphicsCommandList* DXCommon::GetCommandList() const { return commandList_.Get(); }

// CPUのディスクリプターハンドルの取得 
D3D12_CPU_DESCRIPTOR_HANDLE DXCommon::GetCPUDescriptorHandle(
	const ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	// ディスクリプタの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);

	return handleCPU;
}

// GPUのディスクリプターハンドルの取得
D3D12_GPU_DESCRIPTOR_HANDLE DXCommon::GetGPUDescriptorHandle(
	const ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	// ディスクリプタの先頭を取得
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);

	return handleGPU;
}