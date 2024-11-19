#pragma once
/// ===Includ=== ///
// Engine
#include "Engine/Core/ComPtr.h"
// c++
#define DIRECTINPUT_VERSION   0x0800   // DirectInputのバージョン指定
#include <cassert>
#include <dinput.h>
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/// ===前方宣言=== ///
class WinApp;

///=====================================================/// 
/// 入力
///=====================================================///
class Input {

public: // メンバ関数


	Input() = default;
	~Input() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool PushKey(BYTE keyNum);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNum"></param>
	/// <returns></returns>
	bool TriggerKey(BYTE keyNum);

private:
	ComPtr<IDirectInput8> directInput_;
	ComPtr<IDirectInputDevice8> keyboard_;

	BYTE key_[256] = {}; // 全キーの入力情報
	BYTE preKye_[256] = {}; // 前回の全キーの状態
	 
};

