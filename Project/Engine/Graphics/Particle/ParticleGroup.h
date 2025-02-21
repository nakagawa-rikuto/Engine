#pragma once
/// ===incled=== ///
#include "ParticleSetUp.h"
#include <list>

///=====================================================/// 
/// パーティクルのグループ
///=====================================================///
class ParticleGroup {
public:

	ParticleGroup() = default;
	virtual ~ParticleGroup() = default;

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
	struct Group {
		std::unique_ptr<ParticleSetUp> particle; // パーティクルグループ
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
	Group group_{};
	// 時間の進む速度
	const float kDeltaTime_ = 1.0f / 60.0f;
};

