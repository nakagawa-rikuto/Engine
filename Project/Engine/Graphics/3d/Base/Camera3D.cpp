#include "Camera3D.h"

#include <cassert>

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Camera3D::~Camera3D() { buffer_.Reset(); }

///-------------------------------------------/// 
/// 生成
///-------------------------------------------///
void Camera3D::Create(ID3D12Device* device, size_t sizeInBytes) {
	buffer_ = CreateBufferResourceComPtr(device, sizeInBytes);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
ID3D12Resource* Camera3D::GetBuffer() { return buffer_.Get(); }
