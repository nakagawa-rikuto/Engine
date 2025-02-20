#include "ImGuiManager.h"
// Engine
#include "Engine/Core/WinApp.h"
#include "Engine/Core/DXCommon.h"
#include "Engine/System/Managers/SRVManager.h"
// c++
#include <cassert>


///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
ImGuiManager::ImGuiManager() = default;
ImGuiManager::~ImGuiManager() = default;

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ImGuiManager::Initialize(WinApp* winApp, DXCommon* dxCommon, SRVManager* srvManager) {

	assert(winApp);
	assert(dxCommon);
	assert(srvManager);

	winApp_ = winApp;
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

#ifdef USE_IMGUI
	IMGUI_CHECKVERSION(); //　バージョンチェック
	ImGui::CreateContext(); // コンテキストの生成
	ImGui::StyleColorsDark(); // スタイルの設定
	ImGui_ImplWin32_Init(winApp_->GetHwnd()); // Win32用の初期化 
	ImGui_ImplDX12_Init( // DirectX12用の初期化
		dxCommon_->GetDevice(),
		static_cast<int>(dxCommon_->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvManager_->GetDescriptorHeap(),
		srvManager_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		srvManager_->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
#endif // USE_IMGUI

}


///-------------------------------------------/// 
/// 終了
///-------------------------------------------///
void ImGuiManager::Finalize() {
#ifdef USE_IMGUI
	// ImGuiの終了処理.。
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // USE_IMGUI
}


///-------------------------------------------/// 
/// 開始処理
///-------------------------------------------///
void ImGuiManager::Begin() {
#ifdef USE_IMGUI
	// フレームの先頭でImGuiに、ここからフレームが始まる旨を告げる
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の初期に置き換える
	//ImGui::ShowDemoWindow();
#endif // USE_IMGUI
}


///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void ImGuiManager::End() {
#ifdef USE_IMGUI
	// 描画用のDescriptorHeapの設定
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();
	// ImGuiの内部コマンドを生成する
	ImGui::Render();
	// 実際のCommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
#endif // USE_IMGUI
}


///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ImGuiManager::Draw() {
	// 描画用のDescriptorHeapの設定
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager_->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps);
}
