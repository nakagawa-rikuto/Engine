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
///	コントローラー
///=====================================================///
class Controller {
public:

	Controller() =default;
	~Controller();

	// 初期化
	void Initialize(WinApp* winApp, IDirectInput8* input);
	// 更新
	void Update();

	// コントローラーボタンの押下をチェック
	bool PushButton(int deviceIndex, int buttonIndex);
	// コントローラーボタンのトリガーをチェック
	bool TriggerButton(int deviceIndex, int buttonIndex);

private:
	WinApp* winApp_ = nullptr;
	IDirectInput8* directInput_ = nullptr;

	struct ControllerData {
		ComPtr<IDirectInputDevice8> device_; // コントローラー用のDirectInputデバイス
		DIJOYSTATE state_ = {}; // 現在のコントローラーの状態
		DIJOYSTATE preState_ = {}; // 前フレームのコントローラーの状態
	};
	std::vector<ControllerData> controllers_; // 接続されている全コントローラーのリスト

private:
	// ラムダ式の代わりのコールバック関数
	static BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* instance, void* context);
};

