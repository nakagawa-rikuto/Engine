#pragma once
/// ===Include=== ///
// C++
#include <string>
#include <d3d12.h>
// Data
#include "Engine/DataInfo/PipelineStateObjectType.h"


///=====================================================/// 
/// レンダリングサービスロケータ
///=====================================================///
class Render {
public:

	// PSOの取得
	static void SetPSO(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode);
	// SRVインデックス開始番号の取得
	static void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT RootParameterIndex, std::string key);
};

