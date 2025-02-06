#pragma once
/// ===Includ=== ///
#include "Engine/DataInfo/InputData.h"

/// ===前方宣言=== ///
// Input
class Keyboard;
class Mouse;
class Controller;

///=====================================================/// 
/// 入力
///=====================================================///
class Input {

public: // メンバ関数

	// コンストラクタ・デストラクタ
	Input() = default;
	~Input() = default;

	// 初期化
	static void Initialize(Keyboard* keyboard, Mouse* mouse, Controller* controller);
	// 更新
	static void Update();
	// 終了処理
	static void Finalize();

	/// ===キーボード関連=== ///
	// キーの押下をチェック
	static bool PushKey(BYTE keyNum);
	// キーのトリガーをチェック
	static bool TriggerKey(BYTE keyNum);

	/// ===マウス関連=== ///
	// マウスボタンの押下をチェック
	static bool PushMouse(MouseButtonType button);
	// マウスボタンのトリガーをチェック
	static bool TriggerMouse(MouseButtonType button);
	// マウスカーソルの位置を取得（スクリーン座標系）
	static POINT GetMousePosition();
	// マウスのx軸を移動量を取得
	static LONG GetMouseDeltaX();
	// マウスのY軸を移動量を取得
	static LONG GetMouseDeltaY();
	// マウスのスクロール量を取得
	static LONG GetMouseDeltaScroll();

	/// ===コントローラ関連=== ///
	// コントローラーボタンの押下をチェック
	static bool PushButton(int deviceIndex, int buttonIndex);
	// コントローラーボタンのトリガーをチェック
	static bool TriggerButton(int deviceIndex, int buttonIndex);
	// コントローラーの値を取得する	
	//static int GetControllerValue(int deviceIndex, ControllerValueType valueType) const;

private:
	static Keyboard* keyboard_;     // keyboard
	static Mouse* mouse_;           // mouse
	static Controller* controller_; // controller

};

