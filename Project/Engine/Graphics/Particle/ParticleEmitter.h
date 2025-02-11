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
	virtual void Initialze(const std::string& filename) = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw(BlendMode mode) = 0;

public: /// ===Setter=== ///
	// Translate
	virtual void SetTranslate(const Vector3& translate);
	// Rotate
	virtual void SetRotate(const Vector3& rotate);
	// Scale
	virtual void SetScale(const Vector3& scale);

protected:
	/// ===Emitter=== ///
	struct Emitter {
		EulerTransform transform;
		uint32_t count;
		float frequency;
		float frequencyTime;
	};

	// パーティクルの最大数
	uint32_t MaxInstance_;
	uint32_t numInstance_;
	// パーティクル
	std::unique_ptr<ParticleGroup> particle_;
	// ParticleをListで管理する
	std::list<ParticleData> particles_;
	// WorldTransform
	EulerTransform transform_;
	EulerTransform cameraTransform_;
	// エミッタ
	Emitter emitter_{};
	// 時間の進む速度
	const float kDeltaTime_ = 1.0f / 60.0f;
};

