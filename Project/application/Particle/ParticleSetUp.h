#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Core/CData.h"
#include "Engine/Core/ComPtr.h"
// c++
#include <memory>
#include <list>

///=====================================================/// 
/// ParticleSetUp
///=====================================================///
class ParticleSetUp {
public:

	ParticleSetUp();
	~ParticleSetUp();

	// 初期化
	void Initlize(ID3D12Device* device, const uint32_t kNumMaxInstance);

public:
	// リソースの取得
	ID3D12Resource* GetBuffer();

private:

	/// ===バッファリソース=== ///
	ComPtr<ID3D12Resource> buffer_;
	
	/// ===バッファリソース内のデータを指すポインタ=== ///
	ParticleForGPU* data_ = nullptr;

private:

	// Bufferの生成
	void Create(ID3D12Device* device, size_t sizeInBytes);
};

