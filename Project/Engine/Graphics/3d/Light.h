#pragma once
/// ===include=== ///
#include "Engine/Core/ComPtr.h"
#include "Engine/DataInfo/CData.h"

/// ===Lightの種類のenum=== ///
enum class LightType {
	Lambert,
	HalfLambert,
	PointLight,
	SpotLight,
	None
};

///=====================================================/// 
/// Light
///=====================================================///
class Light {
public:
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
};
