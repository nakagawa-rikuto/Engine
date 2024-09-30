#pragma once
#include "DXCommon.h"
#include "PipelineStateObject.h"

/// <summary>
/// スプライト共通武
/// </summary>
class SpriteCommon {
public:/// ===Getter=== ///

	/// <summary>
	/// DXCommonのGetter
	/// </summary>
	/// <returns></returns>
	DXCommon* GetDXCommon() const;

public: /// ===基本的な関数=== ///

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static SpriteCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DXCommon* dxCommon);

public:/// ===共通描画設定=== ///

	/// <summary>
	/// 共通描画設定
	/// </summary>
	void PreDraw();

private:
	// DirectXCommon
	DXCommon* dxCommon_;

	// グラフィックパイプライン
	Microsoft::WRL::ComPtr<PipelineStateObject> pipeline_;
};

