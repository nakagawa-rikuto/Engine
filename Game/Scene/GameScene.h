#pragma once
#include "DXCommon.h"
#include "Input.h"
#include "System.h"
#include "sMath.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: /* メンバ関数 */

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: /* メンバ変数 */
	DXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

