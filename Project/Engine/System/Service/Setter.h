#pragma once
/// ===Include=== ///
// C++
#include <string>
#include <d3d12.h>
// DirectXTex
#include "DirectXTex.h"
// Engine
#include "Engine/DataInfo/CData.h"
#include "Engine/Graphics/OffScreen/OffScreenRenderer.h"

///=====================================================/// 
/// Setter
///=====================================================///
class Setter {
public: /// ===OffScreen=== ///
	static void SetOffScreenType(OffScreenType type);

};

