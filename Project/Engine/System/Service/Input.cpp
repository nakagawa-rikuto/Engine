#include "Input.h"
// c++
#include <cassert>
// Input
#include "Engine/System/Input/Keyboard.h"
#include "Engine/System/Input/Mouse.h"
#include "Engine/System/Input/Controller.h"
// ServiceLocator
#include "ServiceLocator.h"

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Input::Update() {
	ServiceLocator::GetKeyboard()->Update();
	ServiceLocator::GetMouse()->Update();
	ServiceLocator::GetController()->Update();
}

///-------------------------------------------/// 
/// キーボード処理
///-------------------------------------------///
// キーの押下をチェック
bool Input::PushKey(BYTE keyNum) { 
	return ServiceLocator::GetKeyboard()->PushKey(keyNum);
}
// キーのトリガーをチェック
bool Input::TriggerKey(BYTE keyNum) { 
	return ServiceLocator::GetKeyboard()->TriggerKey(keyNum);
}

///-------------------------------------------/// 
/// マウス処理
///-------------------------------------------///
// マウスの押下をチェック
bool Input::PushMouse(MouseButtonType button) {
	return ServiceLocator::GetMouse()->PushMaouseButton(button);
}
// マウスのトリガーをチェック
bool Input::TriggerMouse(MouseButtonType button) { 
	return ServiceLocator::GetMouse()->TriggerMouseButton(button);
}
// マウスカーソルの位置を取得（スクリーン座標系）
POINT Input::GetMousePosition() { 
	return ServiceLocator::GetMouse()->GetMouseCursorPosition();
}
// マウスのX軸移動量を取得
LONG Input::GetMouseDeltaX() { 
	return ServiceLocator::GetMouse()->GetMouseDeltaX();
}
// マウスのY軸移動量を取得
LONG Input::GetMouseDeltaY() { 
	return ServiceLocator::GetMouse()->GetMouseDeltaY();
}
// マウスのスクロール量を取得
LONG Input::GetMouseDeltaScroll() { 
	return ServiceLocator::GetMouse()->GetMouseDeltaScroll();
}

///-------------------------------------------/// 
/// コントローラー処理
///-------------------------------------------///
// コントローラースティックの取得
bool Input::GetJoystickState(int stickNo, XINPUT_STATE& out) { 
	return ServiceLocator::GetController()->GetJoystickState(stickNo, out);
}
bool Input::GetJoystickStatePrevious(int stickNo, XINPUT_STATE& out) { 
	return ServiceLocator::GetController()->GetJoystickStatePrevious(stickNo, out);
}
bool Input::GetJoystickState(int stickNo, DIJOYSTATE2& out) { 
	return ServiceLocator::GetController()->GetJoystickState(stickNo, out);
}
bool Input::GetJoystickStatePrevious(int stickNo, DIJOYSTATE2& out) { 
	return ServiceLocator::GetController()->GetJoystickStatePrevious(stickNo, out);
}
// コントローラーの押下チェック
bool Input::PushButton(int stickNo, ControllerButtonType button) { 
	return ServiceLocator::GetController()->PushButton(stickNo, button);
}
bool Input::TriggerButton(int stickNo, ControllerButtonType button) { 
	return ServiceLocator::GetController()->TriggerButton(stickNo, button);
}
bool Input::ReleaseButton(int stickNo, ControllerButtonType button) { 
	return ServiceLocator::GetController()->ReleaseButton(stickNo, button);
}
// ボタンの押し込み量を取得
float Input::GetTriggerValue(int stickNo, ControllerButtonType button) { 
	return ServiceLocator::GetController()->GetTriggerValue(stickNo, button);
}
// スティックの状況を取得
StickState Input::GetLeftStickState(int stickNo) { 
	return ServiceLocator::GetController()->GetLeftStickState(stickNo);
}
StickState Input::GetRightStickState(int stickNo) { 
	return ServiceLocator::GetController()->GetRightStickState(stickNo);
}
float Input::GetStickValue(int stickNo, ControllerValueType valueType) { 
	return ServiceLocator::GetController()->GetStickValue(stickNo, valueType);
}
