#pragma once
//#include "Mesh.h"

#include "WinApp.h"
#include "DXCommon.h"
//#include "PipelineStateObject.h"

#include <memory>
#include <d3d12.h>

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
	/// Windowsのメッセージを処理する
	/// </summary>
	/// <returns></returns>
	static int ProcessMessage();

private:

	static WinApp* winApp;
	static DXCommon* dXCommon;
};