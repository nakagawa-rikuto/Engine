#include "SelectEmitter.h"
// Math
#include "Math/sMath.h"
// c++
#include <numbers>
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
SelectEmitter::~SelectEmitter() { particle_.reset(); }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SelectEmitter::Initialze(const std::string& filename) {
	std::random_device seedGen;
	randomEngine_.seed(seedGen());

	MaxInstance_ = 200;  // 粒子の最大数
	numInstance_ = 0;

	// デフォルトでは無効
	isActive_ = false;
	aabbMin_ = { 0.0f, 0.0f, 0.0f };
	aabbMax_ = { 0.0f, 0.0f, 0.0f };
	transform_.scale = { 0.5f, 0.5f, 0.5f };

	particle_ = std::make_unique<ParticleGroup>();
	particle_->Initialize(filename, MaxInstance_);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void SelectEmitter::Update() {
	if (!isActive_) return;  // 粒子生成が無効なら処理をスキップ

	numInstance_ = 0;

	// 粒子を生成してAABB内から出す
	while (particles_.size() < MaxInstance_) {
		particles_.push_back(CreateParticle());
	}

	// パーティクルの更新
	for (auto it = particles_.begin(); it != particles_.end();) {
		if (it->currentTime >= it->lifeTime) {
			it = particles_.erase(it);  // 寿命が尽きたパーティクルを削除
			continue;
		}

		// Y方向に移動
		it->transform.translate.y += it->velocity.y * kDeltaTime_;

		// 時間の経過
		it->currentTime += kDeltaTime_;

		// アルファ値を徐々に減少させる
		it->color.w = 1.0f - (it->currentTime / it->lifeTime);

		// WVPマトリクスの計算
		Matrix4x4 worldMatrix = MakeAffineMatrix(it->transform.scale, it->transform.rotate, it->transform.translate);
		Matrix4x4 cameraMatrix = Inverse4x4(MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate));
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 16.0f / 9.0f, 0.1f, 100.0f);
		Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(cameraMatrix, projectionMatrix));

		particle_->SetInstancingData(numInstance_, it->color, wvpMatrix, worldMatrix);
		++numInstance_;
		++it;
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SelectEmitter::Draw(BlendMode mode) {
	if (numInstance_ > 0) {
		particle_->Draw(numInstance_, mode);
	}
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void SelectEmitter::SetPosition(const Vector3& posititon) { transform_.translate = posititon; }
void SelectEmitter::SetAABB(const Vector3& min, const Vector3& max) {
	aabbMin_ = min;
	aabbMax_ = max;
}
void SelectEmitter::SetActive(bool flag) { isActive_ = flag; }

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void SelectEmitter::UpdateImGui() {
	ImGui::Begin("SelectParticle");
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);
	ImGui::DragFloat3("aabbMin", &aabbMin_.x, 0.01f);
	ImGui::DragFloat3("aabbMax", &aabbMax_.x, 0.01f);
	ImGui::Checkbox("isActive", &isActive_);
	ImGui::End();
}

///-------------------------------------------/// 
/// Particle生成ロジック
///-------------------------------------------///
ParticleData SelectEmitter::CreateParticle() {
	std::uniform_real_distribution<float> distX(aabbMin_.x, aabbMax_.x);
	std::uniform_real_distribution<float> distZ(aabbMin_.z, aabbMax_.z);
	std::uniform_real_distribution<float> distLifetime(1.0f, 2.0f);  // 寿命は1～2の間
	std::uniform_real_distribution<float> distScale(0.1f, 0.2f);  // サイズをランダム化

	ParticleData particle;
	particle.transform.scale = { distScale(randomEngine_), distScale(randomEngine_), distScale(randomEngine_) };
	particle.transform.rotate = { 0.0f, 0.0f, 0.0f };

	// AABB内でランダムな初期位置を設定
	particle.transform.translate.x = distX(randomEngine_);
	particle.transform.translate.z = distZ(randomEngine_);
	particle.transform.translate.y = aabbMin_.y;  // Y位置はAABBの下端

	// 上方向の速度を設定
	particle.velocity = { 0.0f, 1.0f, 0.0f };  // Y方向に上昇

	// 初期色と寿命を設定
	particle.color = { 1.0f, 1.0f, 0.5f, 1.0f };  // 初期は明るい色
	particle.lifeTime = distLifetime(randomEngine_);
	particle.currentTime = 0.0f;

	return particle;
}
