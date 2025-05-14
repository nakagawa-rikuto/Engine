#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <d3d12.h>
#include <unordered_map>
#include <functional>

/// ===前方宣言=== ///
class DXCommon;

///=====================================================/// 
/// ルートシグネイチャ
///=====================================================///
class RootSignature {
public:

	RootSignature() = default;
	~RootSignature();

	/// <summary> 
	/// ルートシグネイチャの生成
	/// </summary>
	void Create(DXCommon* dxCommon, PipelineType Type);

	/// <summary>
	/// ルートシグネイチャの取得
	/// </summary>
	/// <returns></returns>
	ID3D12RootSignature* GetRootSignature()const;

private:

	ComPtr<ID3D12RootSignature> rootSignature_; // ルートシグネイチャ

	ComPtr<ID3DBlob> signatureBlob_; // 
	ComPtr<ID3DBlob> errorBlob_; // エラーオブジェクト

	//// ルートシグネチャ構成関数（PipelineTypeごとのディスパッチ）
	//using RootSigGenerator = std::function<ComPtr<ID3D12RootSignature>(ID3D12Device*)>;
	//std::unordered_map<PipelineType, RootSigGenerator> kRootSignatureTable_;
};
