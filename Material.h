#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

#include "CBufferData.h"

class Material {

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Material* GetInstance();

	/// <summary>
	/// マテリアルのリソースの生成
	/// </summary>
	void CreateMaterial(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// マテリアルデータの書き込み
	/// </summary>
	/// <param name="material"></param>
	void WriteMaterial(MaterialData* material);

	/// <summary>
	/// リソースの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetResource();

private:
	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> bufferResource_;

	// データ
	MaterialData* data_;
};

