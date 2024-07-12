
#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

class BlendState {

public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static BlendState* GetInstance();

	/// <summary>
	/// ブレンドステートの設定
	/// </summary>
	/// <returns></returns>
	void Setting();

	/// <summary>
	/// ブレンドステートの取得
	/// </summary>
	/// <returns></returns>
	D3D12_BLEND_DESC GetBlendDesc();

private:

	D3D12_BLEND_DESC blendDesc_{}; // BlendState
};
