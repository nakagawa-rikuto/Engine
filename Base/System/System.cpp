#include "System.h"

#include <d3d12.h>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <span>
#include <dxgidebug.h>
#include <wrl.h>

#include "sMath.h"

WinApp* System::winApp_ = nullptr;
DXCommon* System::dXCommon_ = nullptr;
Input* System::input_ = nullptr;
PipelineStateObject* System::pipeline_ = nullptr;

///=====================================================/// 
/// Sprite
///=====================================================///
// 三角形
Sprite* System::triangleSprite_ = nullptr;
Material* System::materialTriangleSprite_ = nullptr;
Transform* System::wvpTriangleSprite_ = nullptr;

// 四角形
Sprite* System::squareSprite_ = nullptr;
Material* System::materialSquareSprite_ = nullptr;
Transform* System::wvpSquareSprite_ = nullptr;

///=====================================================/// 
/// Mesh
///=====================================================///
Mesh* System::triangle_ = nullptr;

///=====================================================/// 
/// ReportLiveObjects()
///=====================================================///
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

///=====================================================/// 
/// システム全体の初期化
///=====================================================///
void System::Initialize(const wchar_t* title, int width, int height) {

	// ReportLiveObjects
	D3DResourceLeakChecker leakCheck;

	// ゲームウィンドウの作成
	winApp_ = WinApp::GetInstance();
	winApp_->CreateGameWindow(title, width, height);

	// DirectX初期化処理
	dXCommon_ = DXCommon::GetInstance();
	dXCommon_->Initialize(winApp_, width, height);

	// Inputの初期化
	input_ = Input::GetInstance();
	input_->Initialize(winApp_);

	// パイプラインの生成
	pipeline_ = PipelineStateObject::GetInstance();
	assert(dXCommon_->GetDevice());
	pipeline_->CreatePSO(dXCommon_);

	// Triangleの生成
	triangle_ = Mesh::GetInstance();

	///-------------------------------------------/// 
    /// Sprite（三角形）
    ///-------------------------------------------///
	// スプライトの生成
	triangleSprite_ = Sprite::GetInstance();
	triangleSprite_->CreateVertexBuffer(dXCommon_->GetDevice(), sizeof(VertexData) * 3);
	triangleSprite_->CreateIndexBuffer(dXCommon_->GetDevice(), sizeof(uint32_t) * 3);

	// マテリアルの生成
	materialTriangleSprite_ = Material::GetInstance();
	materialTriangleSprite_->CreateResource(dXCommon_->GetDevice(), sizeof(MaterialData) * 3);

	// トランスフォームの生成
	wvpTriangleSprite_ = Transform::GetInstance();
	wvpTriangleSprite_->CreateResource(dXCommon_->GetDevice(), sizeof(TransformationMatrix));

	///-------------------------------------------/// 
    /// Sprite（四角形）
    ///-------------------------------------------///
	// スプライトの生成
	squareSprite_ = Sprite::GetInstance();
	squareSprite_->CreateVertexBuffer(dXCommon_->GetDevice(), sizeof(VertexData) * 6);
	squareSprite_->CreateIndexBuffer(dXCommon_->GetDevice(), sizeof(uint32_t) * 6);

	// マテリアルの生成
	materialSquareSprite_ = Material::GetInstance();
	materialSquareSprite_->CreateResource(dXCommon_->GetDevice(), sizeof(MaterialData) * 3);

	// トランスフォームの生成
	wvpSquareSprite_ = Transform::GetInstance();
	wvpSquareSprite_->CreateResource(dXCommon_->GetDevice(), sizeof(TransformationMatrix));
}

///=====================================================/// 
/// 更新
///=====================================================///
void System::Update() {

	input_->Update();
}

///=====================================================/// 
/// システム全体の終了
///=====================================================///
void System::Finalize() {

	// ゲームウィンドウの破棄
	winApp_->TerminateGameWindow();
}

///=====================================================/// 
/// フレーム開始処理
///=====================================================///
void System::BeginFrame() {
	dXCommon_->PreDraw();
}

///=====================================================/// 
/// フレーム終了処理
///=====================================================///
void System::EndFrame() {
	dXCommon_->PostDraw();
}

///=====================================================/// 
/// Windowsのメッセージを処理する
///=====================================================///
int System::ProcessMessage() { return winApp_->ProcessMessage(); }

///=====================================================/// 
/// 三角形の描画
///=====================================================///
void System::DrawTriangle(
	Vector2* TriangleLeftBottomPositionData,
	Vector2* TriangleTopPositionData,
	Vector2* TriangleRightBottomPositionData) {
	///-------------------------------------------/// 
	/// コマンドリストの生成
	///-------------------------------------------///
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = dXCommon_->GetCommandList();

	///-------------------------------------------/// 
	/// VertexBufferの生成
	///-------------------------------------------///
	// VertexBufferViewの生成
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	// リソースの先頭のアドレスから使用
	vertexBufferView.BufferLocation = triangleSprite_->GetVertexBuffer()->GetGPUVirtualAddress();

	// 使用するリソースのサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 3;

	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	///-------------------------------------------/// 
	/// IndexBufferの生成
	///-------------------------------------------///
	// IndexBufferViewの生成
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	// リソースの先頭のアドレスから使用
	indexBufferView.BufferLocation = triangleSprite_->GetIndexBuffer()->GetGPUVirtualAddress();

	// 使用するリソースのサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 3;

	// １頂点当たりのサイズ
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// データの書き込み
	triangleSprite_->WriteTriangle(TriangleLeftBottomPositionData, TriangleTopPositionData, TriangleRightBottomPositionData);

	///-------------------------------------------/// 
	/// マテリアルの生成
	///-------------------------------------------///
	// マテリアルリソースのデータを設定
	MaterialData material;
	material.color = Vector4(1.0f, 0.0f, 0.0f, 1.0f); // 赤色に設定
	materialTriangleSprite_->WriteData(&material);

	///-------------------------------------------/// 
	/// wvpResourceの生成
	///-------------------------------------------///
	// wvpリソースのデータ設定
	TransformationMatrix wvp;
	wvp.WVP = MakeIdentity4x4(); // 単位行列を入れておく
	wvpTriangleSprite_->WriteData(&wvp);

	///-------------------------------------------/// 
	/// 各々の設定
	///-------------------------------------------///
	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(pipeline_->GetRootSignature());

	// パイプラインステートを設定
	commandList->SetPipelineState(pipeline_->GetPSO());

	// マテリアルリソースを設定
	commandList->SetGraphicsRootConstantBufferView(0, materialTriangleSprite_->GetBuffer()->GetGPUVirtualAddress());

	//　VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView);

	// CBVを設定
	commandList->SetGraphicsRootConstantBufferView(1, wvpTriangleSprite_->GetBuffer()->GetGPUVirtualAddress());

	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 描画コマンド
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);

	/* 三角形の描画をSpriteに変更 */
}

///=====================================================/// 
/// 四角形の描画
///=====================================================///
void System::DrawSquare(
	Vector2* SquareLeftTopPosition,
	Vector2* SquareRightTopPosition,
	Vector2* SquareLeftBottomPosition,
	Vector2* SquareRightBottomPosition) {

	///-------------------------------------------/// 
	/// コマンドリストの生成
	///-------------------------------------------///
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = dXCommon_->GetCommandList();

	///-------------------------------------------/// 
	/// VertexBufferの生成
	///-------------------------------------------///
	// VertexBufferViewの生成
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	// リソースの先頭のアドレスから使用
	vertexBufferView.BufferLocation = squareSprite_->GetVertexBuffer()->GetGPUVirtualAddress();
	// 使用するリソースのサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;

	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	///-------------------------------------------/// 
	/// IndexBufferの生成
	///-------------------------------------------///
	// IndexBufferViewの生成
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	// リソースの先頭のアドレスから使用
	indexBufferView.BufferLocation = squareSprite_->GetIndexBuffer()->GetGPUVirtualAddress();

	// 使用するリソースのサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;

	// １頂点当たりのサイズ
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// データの書き込み
	triangleSprite_->WriteSquare(SquareLeftTopPosition, SquareRightTopPosition, SquareLeftBottomPosition, SquareRightBottomPosition);

	///-------------------------------------------/// 
	/// マテリアルの生成
	///-------------------------------------------///
	// マテリアルリソースのデータを設定
	MaterialData material;
	materialTriangleSprite_->WriteData(&material);
	material.color = Vector4(1.0f, 0.0f, 0.0f, 1.0f); // 赤色に設定

	///-------------------------------------------/// 
	/// wvpResourceの生成
	///-------------------------------------------///
	// wvpリソースのデータ設定
	TransformationMatrix wvp;
	wvpTriangleSprite_->WriteData(&wvp);
	wvp.WVP = MakeIdentity4x4(); // 単位行列を入れておく

	///-------------------------------------------/// 
	/// 各々の設定
	///-------------------------------------------///
	// RootSignature・PipelineStateの設定
	//NOTE:使用するルートシグネイチャやパイプラインステートをそれぞれ変更できるようにするためにここで設定する
	commandList->SetGraphicsRootSignature(pipeline_->GetRootSignature()); // ルートシグネイチャを設定
	commandList->SetPipelineState(pipeline_->GetPSO()); // パイプラインステートを設定

	// マテリアルリソースを設定
	commandList->SetGraphicsRootConstantBufferView(0, materialSquareSprite_->GetBuffer()->GetGPUVirtualAddress());

	//　VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView);

	// CBVを設定
	commandList->SetGraphicsRootConstantBufferView(1, wvpSquareSprite_->GetBuffer()->GetGPUVirtualAddress());

	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 描画コマンド
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	/* 三角形の描画をSpriteに変更 */
}
