#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Graphics/Pipeline/PipelineStateObjectCommon.h"
#include "Engine/Graphics/Pipeline/PipelineStateObjectType.h"
// c++
#include <unordered_map>
#include <memory>
#include <utility> // std::pair
#include <functional> // std::hash

///-------------------------------------------/// 
/// std::pair 用のハッシュ関数を特殊化
///-------------------------------------------///
namespace std {
    template <>
    struct hash<std::pair<PipelineType, BlendMode>> {
        size_t operator()(const std::pair<PipelineType, BlendMode>& p) const noexcept {
            size_t h1 = std::hash<int>()(static_cast<int>(p.first));   // PipelineType のハッシュ値
            size_t h2 = std::hash<int>()(static_cast<int>(p.second)); // BlendMode のハッシュ値
            return h1 ^ (h2 << 1); // ハッシュ値を組み合わせる（XOR とシフト）
        }
    };
}

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
    std::unordered_map<std::pair<PipelineType, BlendMode>, std::unique_ptr<PipelineStateObjectCommon>> pipelines_;

private:/// ===関数=== ///
    /// 指定されたパイプラインを取得
    PipelineStateObjectCommon* GetPipeline(PipelineType type, BlendMode mode);
};