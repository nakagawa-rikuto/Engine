#pragma once
#include "WinApp.h"
#include "DXCommon.h"
#include "PipelineStateObject.h"
#include "Mesh.h"
#include "Material.h"

#include "CBufferData.h"

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
	/// Windowsのメッセージを処理
	/// </summary>
	/// <returns></returns>
	static int ProcessMessage();

	/// <summary>
	/// 三角形の描画
	/// </summary>
	/// <param name="TriangleLeftBottomPositionData"></param>
	/// <param name="TriangleTopPositionData"></param>
	/// <param name="TriangleRightBottomPositionData"></param>
	static void DrawTriangle(
		VertexDataTriangle* TriangleLeftBottomPositionData, VertexDataTriangle* TriangleTopPositionData, VertexDataTriangle* TriangleRightBottomPositionData);

private:

	static WinApp* winApp_;
	static DXCommon* dXCommon_;
	static PipelineStateObject* pipeline_;
	static Mesh* triangle_;
	static Material* materialTriangle_;
};