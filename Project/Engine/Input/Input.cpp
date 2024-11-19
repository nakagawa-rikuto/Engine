#include "Input.h"
// Engine
#include "Engine/Core/WinApp.h"
// Math
#include "Math/sMath.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Input::Initialize(WinApp* winApp) {

	HRESULT hr;

	// DirectInputのインスタンスの生成
	hr = DirectInput8Create(winApp->GetWNDClass().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(hr));

	// キーボードデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);

	// 入力データ形式のセット
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);

	// 排他的正業レベルのセット
	hr = keyboard_->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Input::Update() {

	// 前回のキー入力を保持
	memcpy(preKye_, key_, sizeof(key_));

	// キーボード情報の取得開始
	keyboard_->Acquire();

	// 全キーの入力情報を取得
	keyboard_->GetDeviceState(sizeof(key_), key_);
}

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
