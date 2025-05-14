#pragma once
/// ===Include=== ///
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"

// c++
#include <d3d12.h>
#include <unordered_map>

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
	void Create(PipelineType Type);

	/// <summary>
	/// インプットレイアウトの取得
	/// </summary>
	/// <returns></returns>
	D3D12_INPUT_LAYOUT_DESC GetInputLayout()const;

private:

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{}; // InputLayout

	// 
	struct LayoutInfo {
		const D3D12_INPUT_ELEMENT_DESC* elements;
		UINT numElements;
	};

	// 設定用テーブル
	static const std::unordered_map<PipelineType, LayoutInfo> kLayoutTable_;
};
