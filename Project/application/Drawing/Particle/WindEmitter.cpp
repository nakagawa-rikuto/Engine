#include "WindEmitter.h"
// Engine
#include "Engine/Core/Mii.h"
#include "Engine/System/Service/Getter.h"
// Math
#include "Math/sMath.h"
// c++
#include <numbers>

///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
WindEmitter::WindEmitter() = default;
WindEmitter::~WindEmitter() { emitter_.particle.reset(); }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void WindEmitter::Initialze(const std::string & filename) {

	/// ===乱数生成器の初期化=== ///
	std::random_device seedGenerator;
	randomEngine_.seed(seedGenerator());

	/// ===最大数の設定=== ///
	emitter_.maxInstance = 100;
	emitter_.numInstance = 0;

	/// ===発生頻度の設定=== ///
	emitter_.frequencyCount = 3; // 3個ずつ発生
	emitter_.frequency = 0.5f; // 0.5秒毎に発生
	emitter_.frequencyTime = 0.0f; // 発生頻度ようの時刻。0で初期化

	/// ===Transformの設定=== ///
	emitter_.transform = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0} };
	emitter_.cameraTransform = {
		{1.0f,1.0f,1.0f},
		{std::numbers::pi_v<float> / 3.0f, std::numbers::pi_v<float>, 0.0f },
		{0.0f, 23.0f, 10.0f}
	};

	/// ===Fildの設定=== ///
	accelerationFild_.acceleration = { 15.0f, 0.0f, 0.0f };
	accelerationFild_.area.min = { -1.0f, -1.0f, -1.0f };
	accelerationFild_.area.max = { 1.0f, 1.0f, 1.0f };

	/// ===Emitterの初期化=== ///
	ParticleEmitter::Initialze(filename);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void WindEmitter::InstancingUpdate(std::list<ParticleData>::iterator it) {}
void WindEmitter::Update() {
	// 
	Matrix4x4 cameraMatrix = MakeAffineMatrix(emitter_.cameraTransform.scale, emitter_.cameraTransform.rotate, emitter_.cameraTransform.translate);
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

	// カメラ
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	// Martixの作成
	Matrix4x4 worldMatrix = Multiply(MakeScaleMatrix(emitter_.transform.scale), Multiply(MakeTranslateMatrix(emitter_.transform.translate), billboardMatrix));
	Matrix4x4 viewMatrix = Inverse4x4(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(Getter::GetWindowWidth()) / static_cast<float>(Getter::GetWindowHeight()), 0.1f, 100.0f);

	// インスタンス数を0にする
	emitter_.numInstance = 0;

	// 頻度によって発生させる
	emitter_.frequencyTime += kDeltaTime_; // 時刻を進める
	if (emitter_.frequency <= emitter_.frequencyTime) { // 頻度より大きいなら発生
		emitter_.particles.splice(emitter_.particles.end(), Emit(emitter_, randomEngine_)); // 発生処理
		emitter_.frequencyTime -= emitter_.frequency; // 余計に過ぎたら時間も加味して頻度計算する
	}

	// Particleの処理
	for (std::list<ParticleData>::iterator particleIterator = emitter_.particles.begin(); particleIterator != emitter_.particles.end();) {

		if (emitter_.numInstance < emitter_.maxInstance) {

			if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
				particleIterator = emitter_.particles.erase(particleIterator); // 生存帰還が過ぎたParticleはListから消す。戻り値が次のイテレータとなる
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
			Matrix4x4 worldMatrixs = MakeAffineMatrix((*particleIterator).transform.scale, (*particleIterator).transform.rotate, (*particleIterator).transform.translate);
			Matrix4x4 wvpMatrixs = Multiply(worldMatrixs, Multiply(viewMatrix, projectionMatrix));

			// 値を入力
			emitter_.particle->SetInstancingData(emitter_.numInstance, particleIterator->color, wvpMatrixs, worldMatrixs);
			++emitter_.numInstance;
		}
		++particleIterator; // 次のイテレータに進める
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void WindEmitter::Draw(BlendMode mode) {
	ParticleEmitter::Draw(mode);
}

///-------------------------------------------/// 
/// ランダム発生処理
///-------------------------------------------///
ParticleData WindEmitter::MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate) {
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
std::list<ParticleData> WindEmitter::Emit(const Emitter& emitter, std::mt19937& randomEngine) {
	std::list<ParticleData> particles;
	for (uint32_t count = 0; count < emitter.frequencyCount; ++count) {
		particles.push_back(MakeNewParticle(randomEngine, emitter.transform.translate));
	}
	return particles;
}

///-------------------------------------------/// 
/// AABBの当たり判定
///-------------------------------------------///
bool WindEmitter::IsCollision(const AABB& aabb, const Vector3& point) {
	// 点がAABBの範囲内にあるかどうかを判定
	return (point.x >= aabb.min.x && point.x <= aabb.max.x) &&
		(point.y >= aabb.min.y && point.y <= aabb.max.y) &&
		(point.z >= aabb.min.z && point.z <= aabb.max.z);
}
