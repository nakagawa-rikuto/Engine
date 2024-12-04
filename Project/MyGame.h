#pragma once
/// ===Include=== ///

///=====================================================/// 
/// ゲーム全体
///=====================================================///
class MyGame {
public:

	MyGame();
	~MyGame();

	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 毎フレーム更新
	void Update();
	// 描画
	void Draw();

};

