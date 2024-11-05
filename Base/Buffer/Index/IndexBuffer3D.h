#pragma once
/// ===Include=== ///
#include "Base/Data/CBufferData.h"
#include "Base/ComPtr/ComPtr.h"

///=====================================================/// 
/// IndexBuffer3D
///=====================================================///
class IndexBuffer3D {
public:

	IndexBuffer3D();
	~IndexBuffer3D();

	/// <summary>
	/// Resourceの生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInBytes"></param>
	void Create(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// データの書き込み
	/// </summary>
	/// <param name="transform"></param>
	void WriteData(uint32_t* data);

	/// <summary>
	/// リソースの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetBuffer();

	/// <summary>
	/// データの取得
	/// </summary>
	/// <returns></returns>
	uint32_t* GetData();

	/// <summary>
	/// データのセット
	/// </summary>
	void SetData(uint32_t* data);

private:

	ComPtr<ID3D12Resource> buffer_;

	uint32_t* data_;
};

