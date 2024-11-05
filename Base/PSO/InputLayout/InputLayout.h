#pragma once
/// ===Include=== ///
// Engine
#include "Base/PSO/PipelineStateObjectType.h"

// c++
#include <d3d12.h>

///=====================================================/// 
/// InputLayout
///=====================================================///
class InputLayout {
public:

	InputLayout() = default;
	~InputLayout() = default;

	/// <summary>
	/// InputLayoutを設定
	/// </summary>
	void Create(PipelinType Type);

	/// <summary>
	/// インプットレイアウトの取得
	/// </summary>
	/// <returns></returns>
	D3D12_INPUT_LAYOUT_DESC GetInputLayout()const;

private:

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{}; // InputLayout
};
