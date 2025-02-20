#include "Input.h"
// Input
#include "Engine/System/Input/Keyboard.h"
#include "Engine/System/Input/Mouse.h"
#include "Engine/System/Input/Controller.h"
// c++
#include <cassert>

/// ===宣言=== ///
Keyboard* Input::keyboard_ = nullptr;
Mouse* Input::mouse_ = nullptr;
Controller* Input::controller_ = nullptr;

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Input::Initialize(Keyboard* keyboard, Mouse* mouse, Controller* controller) {
	assert(keyboard);
	assert(mouse);
	assert(controller);

	keyboard_ = keyboard;
	mouse_ = mouse;
	controller_ = controller;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Input::Update() {
	keyboard_->Update();
	mouse_->Update();
	controller_->Update();
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void Input::Finalize() {
	keyboard_ = nullptr;
	mouse_ = nullptr;
	controller_ = nullptr;
}

///-------------------------------------------/// 
/// キーボード処理
///-------------------------------------------///
#pragma region キーボード
// キーの押下をチェック
bool Input::PushKey(BYTE keyNum) { return keyboard_->PushKey(keyNum); }
// キーのトリガーをチェック
bool Input::TriggerKey(BYTE keyNum) { return keyboard_->TriggerKey(keyNum); }
#pragma endregion

///-------------------------------------------/// 
/// マウス処理
///-------------------------------------------///
#pragma region マウス
// マウスの押下をチェック
bool Input::PushMouse(MouseButtonType button) { return mouse_->PushMaouseButton(button); }
// マウスのトリガーをチェック
bool Input::TriggerMouse(MouseButtonType button) { return mouse_->TriggerMouseButton(button); }
// マウスカーソルの位置を取得（スクリーン座標系）
POINT Input::GetMousePosition() { return mouse_->GetMouseCursorPosition(); }
// マウスのX軸移動量を取得
LONG Input::GetMouseDeltaX() { return mouse_->GetMouseDeltaX(); }
// マウスのY軸移動量を取得
LONG Input::GetMouseDeltaY() { return mouse_->GetMouseDeltaY(); }
// マウスのスクロール量を取得
LONG Input::GetMouseDeltaScroll() { return mouse_->GetMouseDeltaScroll(); }
#pragma endregion

///-------------------------------------------/// 
/// コントローラー処理
///-------------------------------------------///
#pragma region コントローラー
// コントローラーの押下をチェック
bool Input::PushButton(int deviceIndex, int buttonIndex) { return controller_->PushButton(deviceIndex, buttonIndex); }
// コントローラーのトリガーをチェック
bool Input::TriggerButton(int deviceIndex, int buttonIndex) { return controller_->TriggerButton(deviceIndex, buttonIndex); }
// コントローラーの値を取得
/*int Input::GetControllerValue(int deviceIndex, ControllerValueType valueType) {}*/
#pragma endregion
