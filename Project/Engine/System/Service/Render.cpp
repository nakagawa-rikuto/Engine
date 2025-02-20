#include "Render.h"
// c++
#include <cassert>
// Manager
#include "Engine/System/Managers/PiplineManager.h"
#include "Engine/System/Managers/TextureManager.h"

/// ===宣言=== ///
PipelineManager* Render::pipelineManager_ = nullptr;
TextureManager* Render::textureManager_ = nullptr;

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Render::Initialize(PipelineManager* pipeline, TextureManager* texture) {
	assert(pipeline);
	assert(texture);

	// 生成
	pipelineManager_ = pipeline;
	textureManager_ = texture;
}

///-------------------------------------------/// 
///	終了処理
///-------------------------------------------///
void Render::Finalize() {
	pipelineManager_ = nullptr;
	textureManager_ = nullptr;
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
#pragma region Pipeline
// PSOの取得
void Render::SetPSO(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode) { pipelineManager_->SetPipeline(commandList, type, mode); }
#pragma endregion
#pragma region Texture
// SRVインデックス開始番号の取得
void Render::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT RootParameterIndex, std::string key) { textureManager_->SetGraphicsRootDescriptorTable(commandList, RootParameterIndex, key); }
#pragma endregion