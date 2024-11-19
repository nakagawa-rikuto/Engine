#pragma once
#include "Base/Data/CBufferData.h"
#include "Base/ComPtr/ComPtr.h"

///=====================================================/// 
/// Light
///=====================================================///
class Light {
public:/// ===基本的な関数=== ///

	Light() = default;
	~Light() = default;

	/// <summary>
	/// 生成
	/// </summary>
	void Create(ID3D12Device* device, size_t sizeInBytes);

public:/// ===Getter=== ///

	/// <summary>
	/// リソースの取得
	/// </summary>
	ID3D12Resource* GetBuffer();

private:/// ===Variables(変数)=== ///

	ComPtr<ID3D12Resource> buffer_;

	DirectionalLight* Data_;
};

