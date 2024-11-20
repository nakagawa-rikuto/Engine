#pragma once
/// ===Includ=== ///
#include "Game/3d/Camera.h"
#include <memory>

///=====================================================/// 
/// ゲームシーン
///=====================================================///
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

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//NOTE:後々作るISceneにデフォルトカメラを入れる
	std::unique_ptr<Camera> defaultCamera_;

};

