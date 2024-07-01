#include "Input.h"

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

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

void Input::Update() {

	// キーボード情報の取得開始
	keyboard_->Acquire();

	// 全キーの入力情報
	BYTE key[256] = {};
	keyboard_->GetDeviceState(sizeof(key), key);
}
