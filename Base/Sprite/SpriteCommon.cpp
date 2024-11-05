#include "SpriteCommon.h"

#include <cassert>

#include "Base/DirectXCommon/DXCommon.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SpriteCommon::Initialize(DXCommon* dxCommon) {

	// 引数で受け取ってメンバ変数に記録する
	dxCommon_ = dxCommon;

	// パイプラインの生成
	pipeline_ = std::make_unique<PipelineStateObjectManager>();
	assert(dxCommon_->GetDevice());
	pipeline_->Create(dxCommon_, PipelinType::Obj2D);
}

///-------------------------------------------/// 
/// 共通描画設定
///-------------------------------------------///
void SpriteCommon::PreDraw() {

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// ルートシグネイチャをセット
	commandList->SetGraphicsRootSignature(pipeline_->GetRootSignature());

	// グラフィックパイプラインステートをセット
	commandList->SetPipelineState(pipeline_->GetPSO());

	// プリミティブトポロジーをセット
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

///-------------------------------------------/// 
/// 共通描画後処理
///-------------------------------------------///
void SpriteCommon::PostDraw() {


}
