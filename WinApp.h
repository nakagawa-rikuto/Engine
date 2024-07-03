#pragma once
// Windows
#include <Windows.h>

// int32_tを使うためのもの
#include <cstdint>

class WinApp {

public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static WinApp* GetInstance();

	/// <summary>
	/// ウィンドウプロージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// ゲームウィンドウの作成関数
	/// </summary>
	/// <param name="kWindowClassName"></param>
	/// <param name="kClientWidth"></param>
	/// <param name="kClientHeight"></param>
	void CreateGameWindow(const wchar_t* title, int32_t kClientWidth, int32_t kClientHeight);

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns></returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd();

private:// メンバ変数

	WNDCLASS wndClass_{}; // ウィンドウクラス
	HWND hwnd_ = nullptr; // ウィンドウハンドル
	UINT windowStyle_; // 
	bool isFullScreen_ = false; // 
	RECT windowRect_; // 
};

