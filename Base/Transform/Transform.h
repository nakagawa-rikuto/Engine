#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

#include "Base/Data/CBufferData.h"

class Transform {

public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Transform* GetInstance();

	/// <summary>
	/// Resourceの生成
	/// </summary>
	void CreateResource(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// データの書き込み
	/// </summary>
	/// <param name="transform"></param>
	void WriteData(TransformationMatrix* data);

	/// <summary>
	/// リソースの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetBuffer();

	/// <summary>
	/// データの取得
	/// </summary>
	/// <returns></returns>
	TransformationMatrix* GetData();

	/// <summary>
	/// データのセット
	/// </summary>
	void SetData(TransformationMatrix* data);
	
private:

	Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;

	TransformationMatrix* data_;
};

