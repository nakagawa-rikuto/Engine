#pragma once
/// ===incled=== ///
#include "ParticleGroup.h"
#include <list>

///=====================================================/// 
/// パーティクルのエミッタ
///=====================================================///
class ParticleEmitter {
public:

	ParticleEmitter() = default;
	virtual ~ParticleEmitter() = default;

	// 初期化
	virtual void Initialze(const std::string& filename);
	// 更新
	virtual void Update(const Vector4& );
	// 描画
	virtual void Draw(BlendMode mode = BlendMode::KBlendModeNormal);

private:
	/// ===Emitter=== ///
	struct Emitter {
		WorldTransform transform;
		uint32_t count;
		float frequency;
		float frequencyTime;
	};

protected:
	// パーティクルの最大数
	uint32_t MaxInstance_;
	// パーティクル
	std::unique_ptr<ParticleGroup> particle_;
	// ParticleをListで管理する
	std::list<ParticleData> particles_;
	// WorldTransform
	WorldTransform transform_;
	WorldTransform cameraTransform_;
	// エミッタ
	Emitter emitter{};
	// 時間の進む速度
	const float kDeltaTime = 1.0 / 60.0;
};

