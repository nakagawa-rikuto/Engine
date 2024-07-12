
#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

class RasterizerState {

public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static RasterizerState* GetInstance();

	/// <summary>
	/// ラスタライザステートの設定
	/// </summary>
	/// <returns></returns>
	void Setting();

	/// <summary>
	/// ラスタライザステートの取得
	/// </summary>
	/// <returns></returns>
	D3D12_RASTERIZER_DESC GetRasterizerState()const;

private:

	D3D12_RASTERIZER_DESC rasterizerDesc_{}; // RasterizerState
};
