#pragma once
/// ===Includ=== ///
// Engine
#include "Engine/Core/ComPtr.h"
#include "Engine/DataInfo/InputData.h"
// c++
#include <array>
#include <vector>
#include <wrl.h>

/// ===前方宣言=== ///
class WinApp;

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

