
#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

class InputLayout {

public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	static InputLayout* GetInstance();

	/// <summary>
	/// InputLayoutを設定
	/// </summary>
	void Setting();

	/// <summary>
	/// インプットレイアウトの取得
	/// </summary>
	/// <returns></returns>
	D3D12_INPUT_LAYOUT_DESC GetInputLayout()const;

private:

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{}; // InputLayout
};
