#pragma once
/// ===Include=== ///
// C++
#include <string>
#include <d3d12.h>
// DirectXTex
#include "DirectXTex.h"
// Engine
#include "Engine/Graphics/OffScreen/OffScreenRenderer.h"

///=====================================================/// 
/// OffScreenService
///=====================================================///
class OffScreenService {
public: /// ===OffScreen=== ///
	static void SetOffScreenType(OffScreenType type);

};

