#pragma once
#define DIRECTINPUT_VERSION   0x0800   // DirectInputのバージョン指定
#include <dinput.h>
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>

#include "WinApp.h"
#include "MyMath.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/// <summary>
/// 入力
/// </summary>
class Input {

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	Microsoft::WRL::ComPtr<IDirectInput8> directInput_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_;
	
};
