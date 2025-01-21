#pragma once
/// ===Include=== ///
// Engien
#include "Engine/Particle/ParticleEmitter.h"
// Math
#include "Math/MathData.h"

///=====================================================/// 
/// 風エミッタ
///=====================================================///
class WindEmitter : public ParticleEmitter {
public:

	WindEmitter();
	~WindEmitter();

	/// ===Override=== ///
	// 初期化
	void Initialze(const std::string& filename)override;
	// 更新
	void Update()override;
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;


private:
	/// ===Field=== ///
	struct AccelerationField {
		Vector3 acceleration;
		AABB area;
	};

	AccelerationField accelerationFild_;
};

