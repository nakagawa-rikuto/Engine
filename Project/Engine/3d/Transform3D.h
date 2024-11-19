#pragma once
/// ===Include=== ///
#include "Engine/Core/CData.h"
#include "Engine/Core/ComPtr.h"

///=====================================================/// 
/// Transform3D
///=====================================================///
class Transform3D {
public:

	Transform3D();
	~Transform3D();

	/// <summary>
	/// Resourceの生成
	/// </summary>
	void Create(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// データの書き込み
	/// </summary>
	/// <param name="transform"></param>
	void WriteData(TransformationMatrix3D* data);

	/// <summary>
	/// リソースの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetBuffer();

	/// <summary>
	/// データの取得
	/// </summary>
	/// <returns></returns>
	TransformationMatrix3D* GetData();

	/// <summary>
	/// データのセット
	/// </summary>
	void SetData(TransformationMatrix3D* data);
	
private:

	ComPtr<ID3D12Resource> buffer_;

	TransformationMatrix3D* data_;
};

