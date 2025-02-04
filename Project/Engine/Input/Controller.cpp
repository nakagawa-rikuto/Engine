#include "Controller.h"
// Engine
#include "Engine/Core/WinApp.h"
#include "InputCommon.h"
// c++
#include <cassert>
// Math
#include "Math/sMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Controller::~Controller() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Controller::Initialize(WinApp * winApp, IDirectInput8 * input) {
	HRESULT hr;
	winApp_ = winApp;
	directInput_ = input;

	// コントローラーの列挙と初期化
	hr = input->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumDevicesCallback,
		this,
		DIEDFL_ATTACHEDONLY
	);
	assert(SUCCEEDED(hr));
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Controller::Update() {
	HRESULT hr;

	// 各コントローラーの状態を取得
	for (auto& controller : controllers_) {
		controller.preState_ = controller.state_; // 前フレームの状態を保持
		hr = controller.device_->Acquire();
		if (SUCCEEDED(hr)) {
			controller.device_->GetDeviceState(sizeof(DIJOYSTATE), &controller.state_);
		}
	}
}

///-------------------------------------------/// 
/// コントローラーの押下チェック
///-------------------------------------------///
bool Controller::PushButton(int deviceIndex, int buttonIndex) {
	if (deviceIndex >= controllers_.size()) return false; // デバイスが範囲外
	return (controllers_[deviceIndex].state_.rgbButtons[buttonIndex] & 0x80) != 0;
}

///-------------------------------------------/// 
/// コントローラーのトリガーチェック
///-------------------------------------------///
bool Controller::TriggerButton(int deviceIndex, int buttonIndex) {
	if (deviceIndex >= controllers_.size()) return false; // デバイスが範囲外
	return (controllers_[deviceIndex].state_.rgbButtons[buttonIndex] & 0x80) &&
		!(controllers_[deviceIndex].preState_.rgbButtons[buttonIndex] & 0x80);
}

///-------------------------------------------/// 
/// スティックの値を取得
///-------------------------------------------///
float Controller::GetStickValue(int deviceIndex, ControllerValueType stickType) {
	if (deviceIndex < 0 || deviceIndex >= static_cast<int>(controllers_.size())) {
		return 0.0f; // 無効なデバイスインデックス
	}

	ControllerData& controller = controllers_[deviceIndex];
	LONG rawValue = 0;

	// DirectInputのスティック値を取得
	switch (stickType) {
	case ControllerValueType::LeftStickX:
		rawValue = controller.state_.lX;
		break;
	case ControllerValueType::LeftStickY:
		rawValue = controller.state_.lY;
		break;
	case ControllerValueType::RightStickX:
		rawValue = controller.state_.lRx;
		break;
	case ControllerValueType::RightStickY:
		rawValue = controller.state_.lRy;
		break;
	default:
		return 0.0f; // 無効なスティックタイプ
	}

	// スティック値の範囲 (-1000 ~ 1000) を -1.0 ~ 1.0 に正規化
	float normalizedValue = static_cast<float>(rawValue) / 1000.0f;

	// デッドゾーン処理
	if (fabs(normalizedValue) < deadZone_) {
		return 0.0f;
	}

	return normalizedValue;
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void Controller::SetDeadZone(const float& deadZone) { deadZone_ = deadZone; }

///-------------------------------------------/// 
/// ラムダ式の代わりのコールバック関数
///-------------------------------------------///
BOOL Controller::EnumDevicesCallback(const DIDEVICEINSTANCE* instance, void* context) {
	// Controller クラスのインスタンスを取得
	auto input = reinterpret_cast<Controller*>(context);

	// DirectInput デバイスを作成
	ComPtr<IDirectInputDevice8> device;
	HRESULT hr = input->directInput_->CreateDevice(instance->guidInstance, &device, nullptr);

	if (SUCCEEDED(hr)) {
		ControllerData controller;
		controller.device_ = device;

		// データフォーマットの設定
		hr = controller.device_->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr)) return DIENUM_CONTINUE;

		// CooperativeLevel の設定
		hr = controller.device_->SetCooperativeLevel(input->winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(hr)) return DIENUM_CONTINUE;

		// コントローラーリストに追加
		input->controllers_.push_back(std::move(controller));
	}

	return DIENUM_CONTINUE; // 列挙を継続
}
