#pragma once
/// ===Include=== ///
// Engien
#include "Engine/Graphics/Particle/Base/ParticleGroup.h"
// Math
#include "Engine/DataInfo/ColliderData.h"
// c++
#include <random>

///=====================================================/// 
/// 風エミッタ
///=====================================================///
class WindParticle : public ParticleGroup {
public:

	WindParticle();
	~WindParticle();

	/// ===Override=== ///
	// 初期化
	void Initialze() override;
	// 更新（override）
	void InstancingUpdate(std::list<ParticleData>::iterator it)override;
	// 更新
	void Update() override;
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;
	// クローン
	std::unique_ptr<ParticleGroup> Clone() override;

private:
	// === コピー禁止 === ///
	WindParticle(const WindParticle&) = delete;
	WindParticle& operator=(const WindParticle&) = delete;

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
	std::list<ParticleData> Emit(const Group& group, std::mt19937& randomEngine);
	// AABBの当たり判定
	bool IsCollision(const AABB& aabb, const Vector3& point);

};

