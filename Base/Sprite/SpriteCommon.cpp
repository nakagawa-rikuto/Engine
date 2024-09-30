#include "SpriteCommon.h"

#include <cassert>

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// DXCommon
DXCommon* SpriteCommon::GetDXCommon() const { return dxCommon_; }

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
SpriteCommon* SpriteCommon::GetInstance() {
	static SpriteCommon instance;
	return &instance;
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SpriteCommon::Initialize(DXCommon* dxCommon) {

	// 引数で受け取ってメンバ変数に記録する
	dxCommon_ = dxCommon;

	// パイプラインの生成
	pipeline_ = PipelineStateObject::GetInstance();
	assert(dxCommon_->GetDevice());
	pipeline_->CreatePSO(dxCommon_);
}

///-------------------------------------------/// 
/// 共通描画設定
///-------------------------------------------///
void SpriteCommon::PreDraw() {

	// ルートシグネイチャをセット
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(pipeline_->GetRootSignature());

	// グラフィックパイプラインステートをセット
	dxCommon_->GetCommandList()->SetPipelineState(pipeline_->GetPSO());

	// プリミティブトポロジーをセット
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
