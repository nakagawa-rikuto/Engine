#pragma once
/// ===Include=== ///
// C++
#include <string>
// Math
#include "Math/Vector3.h"
// BlendModeを使えるようにするため
#include "Engine/DataInfo/PipelineStateObjectType.h"

///=====================================================/// 
/// Particle
///=====================================================///
class Particle {
public:
	// パーティクルの発生
	static void Emit(const std::string& name, const Vector3& translate);
	// テクスチャの設定
	static void SetTexture(const std::string& name, const std::string& textureName);
	// 更新
	static void Update();
	// 描画 
	static void Draw(BlendMode mode = BlendMode::KBlendModeNormal);
};

