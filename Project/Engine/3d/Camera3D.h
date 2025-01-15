#pragma once
/// ===include=== ///
#include "Engine/Core/CData.h"
#include "Engine/Core/ComPtr.h"

///=====================================================/// 
/// カメラBuffer
///=====================================================///
class Camera3D {
public:
	Camera3D() = default;
	~Camera3D() = default;

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

