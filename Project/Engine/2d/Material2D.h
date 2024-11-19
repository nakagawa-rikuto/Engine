#pragma once
/// ===include=== ///
// Engine
#include "Engine/Core/CData.h"
#include "Engine/Core/ComPtr.h"

///=====================================================/// 
/// Material2D
///=====================================================///
class Material2D {
public:

	Material2D();
	~Material2D();

	/// <summary>
	///　リソースの生成
	/// </summary>
	void Create(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// マテリアルデータの書き込み
	/// </summary>
	/// <param name="material"></param>
	void WriteData(MaterialData2D* material);

	/// <summary>
	/// リソースの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetBuffer();

	/// <summary>
	/// データの取得
	/// </summary>
	/// <returns></returns>
	MaterialData2D* GetData();

private:
	// リソース
	
	ComPtr<ID3D12Resource> buffer_;

	// データ
	MaterialData2D* data_;
};

