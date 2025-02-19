#include "Light.h"

#include <cassert>

///-------------------------------------------/// 
/// リソースの生成
///-------------------------------------------///
void Light::Create(ID3D12Device* device, size_t sizeInBytes) {
	buffer_ = CreateBufferResource(device, sizeInBytes);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
ID3D12Resource* Light::GetBuffer() { return buffer_.Get(); }