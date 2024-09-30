#pragma once
#include "WinApp.h"
#include "DXCommon.h"
#include "PipelineStateObject.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Material.h"
#include "Transform.h"

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

	///=====================================================/// 
	/// スプライト
	///=====================================================///
	/// <summary>
	/// 三角形の描画
	/// </summary>
	/// <param name="TriangleLeftBottomPositionData"></param>
	/// <param name="TriangleTopPositionData"></param>
	/// <param name="TriangleRightBottomPositionData"></param>
	static void DrawTriangle(
		Vector2* TriangleLeftBottomPositionData,
		Vector2* TriangleTopPositionData,
		Vector2* TriangleRightBottomPositionData);

	/// <summary>
	/// 四角形の描画
	/// </summary>
	/// <param name="SquareLeftTopPosition"></param>
	/// <param name="SquareRightTopPosition"></param>
	/// <param name="SquareLeftBottomPosition"></param>
	/// <param name="SquareRightBottomPosition"></param>
	static void DrawSquare(
		Vector2* SquareLeftTopPosition,
		Vector2* SquareRightTopPosition,
		Vector2* SquareLeftBottomPosition,
		Vector2* SquareRightBottomPosition);

private:

	static WinApp* winApp_;                  // WinApp
	static DXCommon* dXCommon_;              // DirectXCommon
	static Input* input_;                    // Input
	static PipelineStateObject* pipeline_;   // PSO

	/// ===Sprite=== ///
	// 三角形
	static Sprite* triangleSprite_;
	static Material* materialTriangleSprite_;
	static Transform* wvpTriangleSprite_;

	// 四角形
	static Sprite* squareSprite_;
	static Material* materialSquareSprite_;
	static Transform* wvpSquareSprite_;

	/// ===3dObject=== ///
	static Mesh* triangle_;
};