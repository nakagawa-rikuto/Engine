#pragma once
/// ===Include=== ///
// C++
#include <string>
#include <d3d12.h>
// Data
#include "Engine/DataInfo/PipelineStateObjectType.h"

/// ===前方宣言=== ///
class PipelineManager;
class TextureManager;

///=====================================================/// 
/// レンダリングサービスロケータ
///=====================================================///
class Render {
public:

	Render() = default;
	~Render() = default;

	// 初期化
	static void Initialize(PipelineManager* pipeline, TextureManager* texture);
	// 終了処理
	static void Finalize();
	// PSOの取得
	static void SetPSO(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode);
	// SRVインデックス開始番号の取得
	static void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT RootParameterIndex, std::string key);

private:
	static PipelineManager* pipelineManager_; // PipelineManager
	static TextureManager* textureManager_;   // TextureManager
};

