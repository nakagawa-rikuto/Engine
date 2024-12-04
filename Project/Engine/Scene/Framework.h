#pragma once
/// ===Include=== ///

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

protected:
	// ゲーム終了フラグ
	bool endRequst_ = false;
};

