#include "InputLayout.h"


/// <summary>
/// シングルトンインスタンス
/// </summary>
/// <returns></returns>
InputLayout* InputLayout::GetInstance() {
	static InputLayout instance;
	return &instance;
}

/// <summary>
/// InputLayoutの設定
/// </summary>
void InputLayout::Setting() {

	static D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc_.pInputElementDescs = inputElementDescs;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs);
}

/// <summary>
/// InputLayoutの取得
/// </summary>
D3D12_INPUT_LAYOUT_DESC InputLayout::GetInputLayout() const { return inputLayoutDesc_; }