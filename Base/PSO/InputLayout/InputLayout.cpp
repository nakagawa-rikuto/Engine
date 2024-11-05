#include "InputLayout.h"

///-------------------------------------------/// 
/// InputLayoutの生成
///-------------------------------------------///
void InputLayout::Create(PipelinType Type) {

	if (Type == PipelinType::Obj2D) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_.pInputElementDescs = inputElementDescs;
		inputLayoutDesc_.NumElements = _countof(inputElementDescs);
	} else {
		return;
	}
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
D3D12_INPUT_LAYOUT_DESC InputLayout::GetInputLayout() const { return inputLayoutDesc_; }
