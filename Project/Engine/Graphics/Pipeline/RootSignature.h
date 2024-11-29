
#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
// PipelineType
#include "Game/Data/PipelineStateObjectType.h"

// c++
#include <d3d12.h>

/// ===前方宣言=== ///
class DXCommon;

///=====================================================/// 
/// ルートシグネイチャ
///=====================================================///
class RootSignature {
public:

	RootSignature() = default;
	~RootSignature() = default;

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

};
