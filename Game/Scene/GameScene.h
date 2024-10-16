#pragma once

/// ===前方宣言=== ///
class DXCommon;
class Input;

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

