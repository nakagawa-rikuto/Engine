#pragma once
/// ===include=== ///
#include <memory>

#include "Base/PSO/PipelineStateObjectManager.h"
#include "Base/PSO/PipelineStateObjectType.h"

/// ===前方宣言=== ///
class DXCommon;

/// <summary>
/// スプライト共通武
/// </summary>
class SpriteCommon {

public: /// ===基本的な関数=== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DXCommon* dxCommon);

public:/// ===共通描画設定=== ///

	/// <summary>
	/// 共通描画設定
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 共通描画後処理
	/// </summary>
	void PostDraw();

private:
	// DirectXCommon
	DXCommon* dxCommon_;

	// グラフィックパイプライン
	std::unique_ptr<PipelineStateObjectManager> pipeline_;
};

