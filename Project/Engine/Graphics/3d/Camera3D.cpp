#include "Camera3D.h"

#include <cassert>

///-------------------------------------------/// 
/// 生成
///-------------------------------------------///
void Camera3D::Create(ID3D12Device* device, size_t sizeInBytes) {
	buffer_ = CreateBufferResource(device, sizeInBytes);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
ID3D12Resource* Camera3D::GetBuffer() { return buffer_.Get(); }
