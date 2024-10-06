#pragma once

#include <memory>
#include <d3d12.h>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <span>
#include <dxgidebug.h>
#include <wrl.h>

/// ===前方宣言=== ///
class WinApp;
class DXCommon;
class Input;
class SpriteCommon;

///=====================================================///
/// システム
///=====================================================///
class System {

public:/// ===Getter=== ///

	/// <summary>
	/// SpriteCommonの取得
	/// </summary>
	/// <returns></returns>
	static SpriteCommon* GetSpriteCommon();

public:
	/// <summary>
	/// システム全体の初期化
	/// </summary>
	/// <param name="title"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	static void Initialize(
		const wchar_t* title, int width = 1280, int height = 720);

	/// <summary>
	/// システム全体の更新
	/// </summary>
	static void Update();

	/// <summary>
	/// システム全体の終了
	/// </summary>
	static void Finalize();

	/// <summary>
	/// フレーム開始処理
	/// </summary>
	static void BeginFrame();

	/// <summary>
	/// フレーム終了処理
	/// </summary>
	static void EndFrame();

	/// <summary>
	/// Windowsのメッセージを処理
	/// </summary>
	/// <returns></returns>
	static int ProcessMessage();

private:

	static std::unique_ptr<WinApp> winApp_;           // WinApp
	static std::unique_ptr<DXCommon> dXCommon_;       // DirectXCommon
	static std::unique_ptr<Input> input_;             // Input

	static std::unique_ptr<SpriteCommon> spriteCommon_;      // SpriteCommon
};