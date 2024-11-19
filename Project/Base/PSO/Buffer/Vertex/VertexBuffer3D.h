#pragma once
/// ===Include=== ///
#include "Base/Data/CBufferData.h"
#include "Base/ComPtr/ComPtr.h"

///=====================================================/// 
/// VertexBuffer3D
///=====================================================///
class VertexBuffer3D {
public:

	VertexBuffer3D();
	~VertexBuffer3D();

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
	void WriteData(VertexData3D* data);

	/// <summary>
	/// リソースの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetBuffer();

	/// <summary>
	/// データの取得
	/// </summary>
	/// <returns></returns>
	VertexData3D* GetData();

	/// <summary>
	/// データのセット
	/// </summary>
	void SetData(VertexData3D* data);

private:

	ComPtr<ID3D12Resource> buffer_;

	VertexData3D* data_;
};

