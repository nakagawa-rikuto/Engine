#pragma once
/// ===include=== ///
// C++
#include <memory>
#include <string>
#include <d3d12.h>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <span>
#include <dxgidebug.h>
#include <wrl.h>

/// ===前方宣言=== ///
// Engine
class WinApp;
class DXCommon;
class Input;
// Sprite
class SpriteCommon;
class TextureManager;

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

	/// <summary>
	/// DXCommonの取得
	/// </summary>
	/// <returns></returns>
	static DXCommon* GetDXCommon();

public:/// ===開発者用関数=== ///
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

public:/// ===プログラマー用関数=== ///

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="filePath"></param>
	static void LoadTexture(const std::string& filePath);

	/// <summary>
	/// SRVインデックス開始番号の取得
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	static uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	/// <summary>
	/// GPUハンドルの取得
	/// </summary>
	/// <param name="textureIndex"></param>
	/// <returns></returns>
	static D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(uint32_t textureIndex);

private:

	static std::unique_ptr<WinApp> winApp_;           // WinApp
	static std::unique_ptr<DXCommon> dXCommon_;       // DirectXCommon
	static std::unique_ptr<Input> input_;             // Input

	static std::unique_ptr<TextureManager> textureManager_;   // TextureManager
	static std::unique_ptr<SpriteCommon> spriteCommon_;      // SpriteCommon

};