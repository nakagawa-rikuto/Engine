#pragma once
/// ===Include=== ///
// Engien
#include "Engine/Particle/ParticleEmitter.h"
// Math
#include "Math/MathData.h"
// c++
#include <random>

///=====================================================/// 
/// 風エミッタ
///=====================================================///
class WindEmitter : public ParticleEmitter {
public:

	WindEmitter();
	~WindEmitter();

	/// ===Override=== ///
	// 初期化
	void Initialze(const std::string& filename = "plane")override;
	// 更新
	void Update()override;
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;

	// Setter
	void SetPosition(const Vector3& posititon);

private:
	/// ===Field=== ///
	struct AccelerationField {
		Vector3 acceleration;
		AABB area;
	};

	AccelerationField accelerationFild_;

	// 乱数生成器の初期化
	std::mt19937 randomEngine_;

	

private:

	// ランダムに発生させるための関数
	ParticleData MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate);
	// エミっと
	std::list<ParticleData> Emit(const Emitter& emitter, std::mt19937& randomEngine);
	// AABBの当たり判定
	bool IsCollision(const AABB& aabb, const Vector3& point);

};

