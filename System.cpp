#include "System.h"

#include <d3d12.h>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <span>
#include <dxgidebug.h>
#include <wrl.h>

WinApp* System::winApp_ = nullptr;
DXCommon* System::dXCommon_ = nullptr;
PipelineStateObject* System::pipeline_ = nullptr;
Mesh* System::triangle_ = nullptr;
Material* System::materialTriangle_ = nullptr;

/// <summary>
/// ReportLiveObjects
/// </summary>
struct D3DResourceLeakChecker {

	~D3DResourceLeakChecker() {

		// リソースリークチェック
		Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};


/// <summary>
/// システム全体の初期化
/// </summary>
void System::Initialize(const wchar_t* title, int width, int height) {

	// ReportLiveObjects
	D3DResourceLeakChecker leakCheck;

	// ゲームウィンドウの作成
	winApp_ = WinApp::GetInstance();
	winApp_->CreateGameWindow(title, width, height);

	// DirectX初期化処理
	dXCommon_ = DXCommon::GetInstance();
	dXCommon_->Initialize(winApp_, width, height);

	// パイプラインの生成
	pipeline_ = PipelineStateObject::GetInstance();
	assert(dXCommon_->GetDevice());
	pipeline_->CreatePSO(dXCommon_);

	// Triangleの生成
	triangle_ = Mesh::GetInstance();

	// MaterialTriangleの生成
	materialTriangle_ = Material::GetInstance();
}

/// <summary>
/// システム全体の終了
/// </summary>
void System::Finalize() {

	// ゲームウィンドウの破棄
	winApp_->TerminateGameWindow();
}

/// <summary>
/// フレーム開始処理
/// </summary>
void System::BeginFrame() {
	dXCommon_->PreDraw();
}

/// <summary>
/// フレーム終了処理
/// </summary>
void System::EndFrame() {
	dXCommon_->PostDraw();
}

/// <summary>
/// Windowsのメッセージを処理する
/// </summary>
int System::ProcessMessage() { return winApp_->ProcessMessage(); }

/// <summary>
/// 三角形の描画
/// </summary>
void System::DrawTriangle(
	VertexDataTriangle* TriangleLeftBottomPositionData, VertexDataTriangle* TriangleTopPositionData, VertexDataTriangle* TriangleRightBottomPositionData) {

	// コマンドリストの生成
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = dXCommon_->GetCommandList();

	// VertexResourceの生成
	triangle_->CreateVertexBuffer(dXCommon_->GetDevice(), sizeof(VertexDataTriangle));

	// データの書き込み
	triangle_->WriteVertexBufferTriangle(TriangleLeftBottomPositionData, TriangleTopPositionData, TriangleRightBottomPositionData);

	// マテリアルリソースの生成
	materialTriangle_->CreateMaterial(dXCommon_->GetDevice(), sizeof(MaterialData));

	// マテリアルリソースのデータを設定
	MaterialData data;
	data.color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	materialTriangle_->WriteMaterial(&data);

	// VertexBufferViewの生成
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = triangle_->GetVertexBufferView();
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = triangle_->GetVertexBuffer()->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexDataTriangle) * 3;
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexDataTriangle);

	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(pipeline_->GetRootSignature());

	// パイプラインステートを設定
	commandList->SetPipelineState(pipeline_->GetPSO());

	// マテリアルリソースを設定
	commandList->SetGraphicsRootConstantBufferView(0, materialTriangle_->GetResource()->GetGPUVirtualAddress());

	// 頂点バッファを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 描画コマンド
	commandList->DrawInstanced(3, 1, 0, 0);
}
