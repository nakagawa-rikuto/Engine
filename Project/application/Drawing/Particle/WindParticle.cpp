#include "WindParticle.h"
// c++
#include <numbers>
// Service
#include "Engine/System/Service/Getter.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void WindParticle::Initialze() {

	/// ===乱数生成器の初期化=== ///
	std::random_device seedGenerator;
	randomEngine_.seed(seedGenerator());

	/// ===最大数の設定=== ///
	group_.maxInstance = 100;
	group_.numInstance = 0;

	/// ===発生頻度の設定=== ///
	group_.frequencyCount = 3; // 3個ずつ発生
	group_.frequency = 0.5f; // 0.5秒毎に発生
	group_.frequencyTime = 0.0f; // 発生頻度ようの時刻。0で初期化

	/// ===Transformの設定=== ///
	group_.transform = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0} };
	group_.cameraTransform = {
		{1.0f,1.0f,1.0f},
		{std::numbers::pi_v<float> / 3.0f, std::numbers::pi_v<float>, 0.0f },
		{0.0f, 23.0f, 10.0f}
	};

	/// ===Fildの設定=== ///
	accelerationFild_.acceleration = { 15.0f, 0.0f, 0.0f };
	accelerationFild_.area.min = { -1.0f, -1.0f, -1.0f };
	accelerationFild_.area.max = { 1.0f, 1.0f, 1.0f };

	/// ===パーティクルグループの初期化=== ///
	group_.particle = std::make_unique<ParticleSetUp>();
	group_.particle->Initialze("plane", group_.maxInstance); /*"Particle"*/
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void WindParticle::InstancingUpdate(std::list<ParticleData>::iterator it) {}
void WindParticle::Update() {
	// 
	Matrix4x4 cameraMatrix = Math::MakeAffineEulerMatrix(group_.cameraTransform.scale, group_.cameraTransform.rotate, group_.cameraTransform.translate);
	Matrix4x4 backToFrontMatrix = Math::MakeRotateYMatrix(std::numbers::pi_v<float>);

	// カメラ
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	// Martixの作成
	Matrix4x4 worldMatrix = Multiply(Math::MakeScaleMatrix(group_.transform.scale), Multiply(Math::MakeTranslateMatrix(group_.transform.translate), billboardMatrix));
	Matrix4x4 viewMatrix = Math::Inverse4x4(cameraMatrix);
	Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(0.45f, static_cast<float>(Getter::GetWindowWidth()) / static_cast<float>(Getter::GetWindowHeight()), 0.1f, 100.0f);

	// インスタンス数を0にする
	group_.numInstance = 0;

	// 頻度によって発生させる
	group_.frequencyTime += kDeltaTime_; // 時刻を進める
	if (group_.frequency <= group_.frequencyTime) { // 頻度より大きいなら発生
		group_.particles.splice(group_.particles.end(), Emit(group_, randomEngine_)); // 発生処理
		group_.frequencyTime -= group_.frequency; // 余計に過ぎたら時間も加味して頻度計算する
	}

	// Particleの処理
	for (std::list<ParticleData>::iterator particleIterator = group_.particles.begin(); particleIterator != group_.particles.end();) {

		if (group_.numInstance < group_.maxInstance) {

			if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
				particleIterator = group_.particles.erase(particleIterator); // 生存帰還が過ぎたParticleはListから消す。戻り値が次のイテレータとなる
				continue;
			}

			// Fieldの範囲内のParticleには加速度を適用する
			if (IsCollision(accelerationFild_.area, (*particleIterator).transform.translate)) {
				(*particleIterator).velocity += accelerationFild_.acceleration + kDeltaTime_;
			}

			// 速度の設定
			(*particleIterator).transform.translate += (*particleIterator).velocity * kDeltaTime_;
			(*particleIterator).currentTime += kDeltaTime_;

			// alpha値の計算
			float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
			(*particleIterator).color.w = alpha;

			// WVPMatrix
			Matrix4x4 worldMatrixs = Math::MakeAffineEulerMatrix((*particleIterator).transform.scale, (*particleIterator).transform.rotate, (*particleIterator).transform.translate);
			Matrix4x4 wvpMatrixs = Multiply(worldMatrixs, Multiply(viewMatrix, projectionMatrix));

			// 値を入力
			group_.particle->SetInstancingData(group_.numInstance, particleIterator->color, wvpMatrixs, worldMatrixs);
			++group_.numInstance;
		}
		++particleIterator; // 次のイテレータに進める
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void WindParticle::Draw(BlendMode mode) {
	ParticleGroup::Draw(mode);
}

///-------------------------------------------/// 
/// ランダム発生処理
///-------------------------------------------///
ParticleData WindParticle::MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate) {
	std::uniform_int_distribution<int> distribution(-1, 1);
	std::uniform_int_distribution<int> distColor(0, 1);
	std::uniform_int_distribution<int> distTime(1, 3);

	ParticleData particle;
	particle.transform.scale = { 1.0f, 1.0f, 1.0f };
	particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
	Vector3 randomTranslate = { static_cast<float>(distribution(randomEngine)), static_cast<float>(distribution(randomEngine)), static_cast<float>(distribution(randomEngine)) };
	particle.transform.translate = randomTranslate + translate;
	particle.velocity = { static_cast<float>(distribution(randomEngine)), static_cast<float>(distribution(randomEngine)), static_cast<float>(distribution(randomEngine)) };
	particle.color = { static_cast<float>(distColor(randomEngine)), static_cast<float>(distColor(randomEngine)) , static_cast<float>(distColor(randomEngine)) , static_cast<float>(distColor(randomEngine)) };
	particle.lifeTime = static_cast<float>(distTime(randomEngine));
	particle.currentTime = 0.0f;

	return particle;
}

///-------------------------------------------/// 
/// エミっと
///-------------------------------------------///
std::list<ParticleData> WindParticle::Emit(const Group& group, std::mt19937& randomEngine) {
	std::list<ParticleData> particles;
	for (uint32_t count = 0; count < group.frequencyCount; ++count) {
		particles.push_back(MakeNewParticle(randomEngine, group.transform.translate));
	}
	return particles;
}

///-------------------------------------------/// 
/// AABBの当たり判定
///-------------------------------------------///
bool WindParticle::IsCollision(const AABB& aabb, const Vector3& point) {
	// 点がAABBの範囲内にあるかどうかを判定
	return (point.x >= aabb.min.x && point.x <= aabb.max.x) &&
		(point.y >= aabb.min.y && point.y <= aabb.max.y) &&
		(point.z >= aabb.min.z && point.z <= aabb.max.z);
}
