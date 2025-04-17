#pragma once
/// ===incled=== ///
#include "ParticleSetUp.h"
#include <list>

/// ===Camera=== ///
class Camera;

///=====================================================/// 
/// パーティクルのグループ
///=====================================================///
class ParticleGroup {
public:

	ParticleGroup() = default;
	virtual ~ParticleGroup();

	// 初期化
	virtual void Initialze() = 0;
	// 更新
	virtual void InstancingUpdate(std::list<ParticleData>::iterator it);
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw(BlendMode mode);
	// クローン 
	virtual std::unique_ptr<ParticleGroup> Clone() = 0;
	// 生存判定
	virtual bool IsFinish();

public: /// ===Setter=== ///
	// Translate
	void SetTranslate(const Vector3& translate);
	// Rotate
	void SetRotate(const Vector3& rotate);
	// Scale
	void SetScale(const Vector3& scale);
	// Texture
	void SetTexture(const std::string& fileName);
	// Camera
	void SetCamera(Camera* camera);

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
		Camera* camera = nullptr;
	};

	// エミッタ
	Group group_{};
	// 時間の進む速度
	const float kDeltaTime_ = 1.0f / 60.0f;
};

