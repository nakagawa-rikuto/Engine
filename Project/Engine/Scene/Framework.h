#pragma once
/// ===Include=== ///
// SceneFactory
#include "Engine/Scene/AbstractSceneFactory.h"
// ｃ++
#include <memory>

///=====================================================/// 
/// フレームワーク
///=====================================================///
class Framework {
public:/// ====== ///

	Framework() = default;
	virtual ~Framework() = default;

	// 初期化
	virtual void Initialize(const wchar_t* title);
	// 終了
	virtual void Finalize();
	// 毎フレーム更新
	virtual void Update();
	// 描画
	virtual void Draw() = 0;
	// 終了フラグのチェック
	virtual bool IsEndRequst();

public:/// ===メンバ関数=== ///
	// 実行
	void Run(const wchar_t* title);
	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();

protected:
	// ゲーム終了フラグ
	bool endRequst_ = false;
	// シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;
};

