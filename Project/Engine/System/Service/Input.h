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

	/// ===キーボード=== ///
	// キーの押下をチェック
	static bool PushKey(BYTE keyNum);
	// キーのトリガーをチェック
	static bool TriggerKey(BYTE keyNum);

	/// ===マウス=== ///
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

	/// ===コントローラ=== ///
	// コントローラースティックの取得
	static bool GetJoystickState(int32_t stickNo, XINPUT_STATE& out);
	static bool GetJoystickStatePrevious(int32_t stickNo, XINPUT_STATE& out);
	static bool GetJoystickState(int32_t stickNo, DIJOYSTATE2& out);
	static bool GetJoystickStatePrevious(int32_t stickNo, DIJOYSTATE2& out);

	// コントローラーの押下チェック
	static bool PushButton(int32_t stickNo, ControllerButtonType button);
	static bool TriggerButton(int32_t stickNo, ControllerButtonType button);
	static bool ReleaseButton(int32_t stickNo, ControllerButtonType button);

	// ボタンの押し込み量を取得
	static float GetTriggerValue(int32_t stickNo, ControllerButtonType button);

	// スティックの状況を取得
	static StickState GetLeftStickState(int32_t stickNo);
	static StickState GetRightStickState(int32_t stickNo);
	static float GetStickValue(int32_t stickNo, ControllerValueType valueType);

private:
	static Keyboard* keyboard_;     // keyboard
	static Mouse* mouse_;           // mouse
	static Controller* controller_; // controller
};

