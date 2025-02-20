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
	virtual void InstancingUpdate(std::list<ParticleData>::iterator it);
	// 描画
	virtual void Draw(BlendMode mode);

public: /// ===Setter=== ///
	// Translate
	void SetTranslate(const Vector3& translate);
	// Rotate
	void SetRotate(const Vector3& rotate);
	// Scale
	void SetScale(const Vector3& scale);
	// Texture
	void SetTexture(const std::string& fileName);

protected:
	/// ===Emitter=== ///
	struct Emitter {
		std::unique_ptr<ParticleGroup> particle; // パーティクルグループ
		std::list<ParticleData> particles; // パーティクルのリスト
		EulerTransform transform; // エミッタのTransform
		EulerTransform cameraTransform; // カメラのTransform
		uint32_t maxInstance; // パーティクルの最大数
		uint32_t numInstance; // インスタンス数
		uint32_t frequencyCount; // パーティクルの発生頻度のカウント
		float frequency; // パーティクルの発生頻度
		float frequencyTime; // パーティクルの発生頻度の時間
	};

	// エミッタ
	Emitter emitter_{};
	// 時間の進む速度
	const float kDeltaTime_ = 1.0f / 60.0f;
};

