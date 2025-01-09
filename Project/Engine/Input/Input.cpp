#include "Input.h"
// Engine
#include "Engine/Core/WinApp.h"
// c++
#include <cassert>
// Math
#include "Math/sMath.h"

///-------------------------------------------///  
/// デストラクタ
///-------------------------------------------///
Input::~Input() {
	if (keyboard_) {
		keyboard_->Unacquire();
	}
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Input::Initialize(WinApp* winApp) {

	HRESULT hr;
	winApp_ = winApp;

	// DirectInputのインスタンスの生成
	hr = DirectInput8Create(winApp_->GetWNDClass().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(hr));

	/// ===キーボード関連=== ///
	// キーボードデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, nullptr);
	assert(SUCCEEDED(hr));
	// 入力データ形式のセット
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	// 排他的制御レベルのセット
	hr = keyboard_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

	/// ===マウス関連=== ///
	// マウスデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysMouse, &mouse_, nullptr);
	assert(SUCCEEDED(hr));
	// 入力データ形式のセット
	hr = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(hr));
	// 排他的制御レベルのセット
	hr = mouse_->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(hr));

	/// ===コントローラー関連=== ///
	 // コントローラーの列挙と初期化
	hr = directInput_->EnumDevices(
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
void Input::Update() {

	HRESULT hr;

	// 前回のキー入力を保持
	memcpy(preKye_, key_, sizeof(key_));
	preMouseState_ = mouseState_;

	// キーボード情報の取得開始
	hr = keyboard_->Acquire();
	if (SUCCEEDED(hr)) {
		keyboard_->GetDeviceState(sizeof(key_), key_);
	}

	// マウスの状態を取得
	hr = mouse_->Acquire();
	if (SUCCEEDED(hr)) {
		mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);
	}

	// 各コントローラーの状態を取得
	for (auto& controller : controllers_) {
		controller.preState_ = controller.state_; // 前フレームの状態を保持
		hr = controller.device_->Acquire();
		if (SUCCEEDED(hr)) {
			controller.device_->GetDeviceState(sizeof(DIJOYSTATE), &controller.state_);
		}
	}

	// 全キーの入力情報を取得
	hr = keyboard_->GetDeviceState(sizeof(key_), key_);
	//assert(SUCCEEDED(hr));
}

#pragma region キーボード関連
///-------------------------------------------/// 
/// キーの押下をチェック
///-------------------------------------------///
bool Input::PushKey(BYTE keyNum) {
	// 指定キーを押して入ればTrueを返す
	if (key_[keyNum]) {
		return true;
	}

	// そうでなければFalseを返す
	return false;
}

///-------------------------------------------/// 
/// キーのトリガーをチェック
///-------------------------------------------///
bool Input::TriggerKey(BYTE keyNum) {

	if (key_[keyNum] && !preKye_[keyNum]) {
		return true;
	}

	return false;
}
#pragma endregion

#pragma region マウス関連
///-------------------------------------------/// 
/// マウスの押下をチェック
///-------------------------------------------///
bool Input::PushMaouseButton(MouseButtonType button) {
	// マウスボタンが押されている場合にtrueを返す
	return (mouseState_.rgbButtons[static_cast<int>(button)] & 0x80) != 0;
}

///-------------------------------------------/// 
/// マウスのトリガーをチェック
///-------------------------------------------///
bool Input::TriggerMouseButton(MouseButtonType button) {
	// マウスボタンがトリガー（押された瞬間）の場合にtrueを返す
	return (mouseState_.rgbButtons[static_cast<int>(button)] & 0x80) &&
		!(preMouseState_.rgbButtons[static_cast<int>(button)] & 0x80);
}

///-------------------------------------------/// 
/// マウスカーソルの位置を取得（スクリーン座標系）
///-------------------------------------------///
POINT Input::GetMouseCursorPosition() const {
	POINT cursorPosition{};
	if (GetCursorPos(&cursorPosition)) {
		// スクリーン座標からクライアント座標へ変換
		if (ScreenToClient(winApp_->GetHwnd(), &cursorPosition)) {
			return cursorPosition; // マウスカーソルのクライアント座標を返す
		}
	}
	return { -1, -1 }; // 失敗した場合のデフォルト値
}

///-------------------------------------------/// 
/// マウスのX軸移動量を取得
///-------------------------------------------///
LONG Input::GetMouseDeltaX() const {
	// マウスのX軸の移動量を返す
	return mouseState_.lX;
}

///-------------------------------------------/// 
/// マウスのY軸移動量を取得
///-------------------------------------------///
LONG Input::GetMouseDeltaY() const {
	// マウスのY軸の移動量を返す
	return mouseState_.lY;
}

///-------------------------------------------/// 
/// マウスのスクロール量を取得
///-------------------------------------------///
LONG Input::GetMouseDeltaScroll() const {
	// マウスのスクロール量を返す
	return mouseState_.lZ;
}
#pragma endregion

#pragma region コントローラー関連
///-------------------------------------------/// 
/// コントローラーの押下をチェック
///-------------------------------------------///
bool Input::PushButton(int deviceIndex, int buttonIndex) {
	if (deviceIndex >= controllers_.size()) return false; // デバイスが範囲外
	return (controllers_[deviceIndex].state_.rgbButtons[buttonIndex] & 0x80) != 0;
}

///-------------------------------------------/// 
/// コントローラーのトリガーをチェック
///-------------------------------------------///
bool Input::TriggerButton(int deviceIndex, int buttonIndex) {
	if (deviceIndex >= controllers_.size()) return false; // デバイスが範囲外
	return (controllers_[deviceIndex].state_.rgbButtons[buttonIndex] & 0x80) &&
		!(controllers_[deviceIndex].preState_.rgbButtons[buttonIndex] & 0x80);
}

///-------------------------------------------/// 
/// コントローラーの値を取得
///-------------------------------------------///
//int Input::GetControllerValue(int deviceIndex, ControllerValueType valueType) const {
//	// インデックスの範囲チェック
//	if (index < 0 || deviceIndex >= static_cast<int>(controllers_.size())) {}
//}

///-------------------------------------------/// 
/// ラムダ式の代わりのコールバック関数
///-------------------------------------------///
BOOL Input::EnumDevicesCallback(const DIDEVICEINSTANCE* instance, void* context) {
	// Input クラスのインスタンスを取得
	auto input = reinterpret_cast<Input*>(context);

	// DirectInput デバイスを作成
	ComPtr<IDirectInputDevice8> device;
	HRESULT hr = input->directInput_->CreateDevice(instance->guidInstance, &device, nullptr);

	if (SUCCEEDED(hr)) {
		Controller controller;
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
#pragma endregion
