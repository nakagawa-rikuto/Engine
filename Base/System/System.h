#pragma once
#include "WinApp.h"
#include "DXCommon.h"
#include "PipelineStateObject.h"

#include "Input.h"
#include "CBufferData.h"

#include <memory>
#include <d3d12.h>

///=====================================================///
/// システム
///=====================================================///
class System {

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

	static WinApp* winApp_;                  // WinApp
	static DXCommon* dXCommon_;              // DirectXCommon
	static Input* input_;                    // Input
	static PipelineStateObject* pipeline_;   // PSO
};