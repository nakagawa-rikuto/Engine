#pragma once
/// ===Includ=== ///
// Engine
#include "Engine/Core/ComPtr.h"
// c++
#define DIRECTINPUT_VERSION   0x0800   // DirectInputのバージョン指定
#include <dinput.h>
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/// ===前方宣言=== ///
class WinApp;

/// ===マウスのキーを指定する為の列挙型=== ///
enum class MouseButtonType {
	Left,       // 左クリック
	Right,      // 右クリック
	Scroll,     // スクロールボタン
	Button1,    // ボタン1
};

/// ===コントローラーのキーを指定するための列挙型=== ///
enum class ControllerValueType {
	LeftStickX, // 左スティックのX軸
	LeftStickY, // 左スティックのY軸
	RightStickX,// 右スティックのX軸
	RightStickY,// 右スティックのY軸
	Top,        // 上ボタン 
	Bottom,     // 下ボタン
	Left,       // 左ボタン
	Right,      // 右ボタン
	A,          // Aボタン
	B,          // Bボタン
	X,          // Xボタン
	Y,          // Yボタン
	RB,         // RBボタン
	RT,         // RTボタン
	LB,         // LBボタン
	LT,         // LTボタン
	LeftStick,  // Lスティックの押し込み
	RightStick, // Rスティックの押し込み
};

///=====================================================/// 
/// 入力
///=====================================================///
class Input {

public: // メンバ関数

	// コンストラクタ・デストラクタ
	Input() = default;
	~Input();

	// 初期化
	void Initialize(WinApp* winApp);
	// 更新
	void Update();

	/// ===キーボード関連=== ///
	// キーの押下をチェック
	bool PushKey(BYTE keyNum);
	// キーのトリガーをチェック
	bool TriggerKey(BYTE keyNum);

	/// ===マウス関連=== ///
	// マウスボタンの押下をチェック
	bool PushMaouseButton(MouseButtonType button);
	// マウスボタンのトリガーをチェック
	bool TriggerMouseButton(MouseButtonType button);
	// マウスカーソルの位置を取得（スクリーン座標系）
	POINT GetMouseCursorPosition() const;
	// マウスのx軸を移動量を取得
	LONG GetMouseDeltaX() const;
	// マウスのY軸を移動量を取得
	LONG GetMouseDeltaY() const;
	// マウスのスクロール量を取得
	LONG GetMouseDeltaScroll() const;

	/// ===コントローラ関連=== ///
	// コントローラーボタンの押下をチェック
	bool PushButton(int deviceIndex, int buttonIndex);
	// コントローラーボタンのトリガーをチェック
	bool TriggerButton(int deviceIndex, int buttonIndex);
	// コントローラーの値を取得する	
	//int GetControllerValue(int deviceIndex, ControllerValueType valueType) const;

private:
	WinApp* winApp_ = nullptr; // ウィンドウプロージャのポインタ
	ComPtr<IDirectInput8> directInput_; // DirectInputインスタンス

	/// ===キーボード関連=== ///
	ComPtr<IDirectInputDevice8> keyboard_; // キーボード用のDirectInputデバイス
	BYTE key_[256] = {}; // 現在の全キーの状態
	BYTE preKye_[256] = {}; // 前回の全キーの状態

	/// ===マウス関連=== ///
	ComPtr<IDirectInputDevice8> mouse_; // マウス用のDirectInputデバイス
	DIMOUSESTATE mouseState_ = {}; // 現在のマウスの状態
	DIMOUSESTATE preMouseState_ = {}; // 前フレームのマウスの状態
	//NOTE:今回はDIMOUSESTATEを使用しているがマウスのボタンの数が４つじゃなりないのならDIMOUSESTATE2を使用すると良い

	/// ===コントローラー関連=== ///
	struct Controller {
		ComPtr<IDirectInputDevice8> device_; // コントローラー用のDirectInputデバイス
		DIJOYSTATE state_ = {}; // 現在のコントローラーの状態
		DIJOYSTATE preState_ = {}; // 前フレームのコントローラーの状態
	};
	std::vector<Controller> controllers_; // 接続されている全コントローラーのリスト

private:
	// ラムダ式の代わりのコールバック関数
	static BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* instance, void* context);
};

