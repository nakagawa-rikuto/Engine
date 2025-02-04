#pragma once
/// ===Include=== ///
#define DIRECTINPUT_VERSION   0x0800   // DirectInputのバージョン指定
#include <dinput.h>
#include <Windows.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

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
