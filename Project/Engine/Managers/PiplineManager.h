#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Graphics/Pipeline/PipelineStateObjectCommon.h"
#include "Game/Data/PipelineStateObjectType.h"
// c++
#include <unordered_map>
#include <memory>

class PipelineManager {
public:
    
    PipelineManager();
    ~PipelineManager();

    // 初期化
    void Initialize();
    // PSOの設定
    void SetPipeline(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode);

private:/// ===変数=== ///
    /// パイプラインの管理
    std::unordered_map<std::pair<PipelineType, BlendMode>, std::unique_ptr<PipelineStateObjectCommon>, std::hash<int>> pipelines_;

private:/// ===関数=== ///
    /// 指定されたパイプラインを取得
    PipelineStateObjectCommon* GetPipeline(PipelineType type, BlendMode mode);
};