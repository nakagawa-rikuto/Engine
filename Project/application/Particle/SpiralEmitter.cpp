#include "SpiralEmitter.h"
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
SpiralEmitter::~SpiralEmitter() { particle_.reset(); }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SpiralEmitter::Initialze(const std::string & filename) {
    std::random_device seedGen;
    randomEngine_.seed(seedGen());

    MaxInstance_ = 500;   // 円を構成するパーティクル数
    numInstance_ = 0;

    verticalSpeed_ = 2.5f;    // 上昇速度
    maxLifetime_ = 3.0f;      // パーティクルの寿命
    spiralRadius_ = 1.0f;     // 円の半径
    emitInterval_ = 0.00001f;    // パーティクルの生成間隔
    rotationSpeed_ = 1000.0f;    // 回転速度（高く設定）
    spiralRadiusMax_ = 1.5f;  // 最大半径
    spiralRadiusMin_ = 0.3f;  // 最小半径
    spiralRadiusSpeed_ = 0.5f;  // 半径変化速度

    elapsedTime_ = 0.0f;      // 経過時間の初期化
    currentAngle_ = 0.0f;     // 円周上の初期角度

    isExpanding_ = true;  // 拡大中

    transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

    particle_ = std::make_unique<ParticleGroup>();
    particle_->Initialize(filename, MaxInstance_);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void SpiralEmitter::Update() {
    elapsedTime_ += kDeltaTime_;

    // 螺旋半径の拡大・縮小処理
    if (isExpanding_) {
        spiralRadius_ += spiralRadiusSpeed_ * kDeltaTime_;
        if (spiralRadius_ >= spiralRadiusMax_) {
            isExpanding_ = false;  // 縮小に切り替え
        }
    } else {
        spiralRadius_ -= spiralRadiusSpeed_ * kDeltaTime_;
        if (spiralRadius_ <= spiralRadiusMin_) {
            isExpanding_ = true;  // 拡大に切り替え
        }
    }

    // 一定間隔でパーティクルを生成
    if (elapsedTime_ >= emitInterval_ && particles_.size() < MaxInstance_) {
        particles_.push_back(CreateParticle(transform_.translate, currentAngle_));
        currentAngle_ += (rotationSpeed_ * kDeltaTime_);  // 回転速度に基づく角度増加
        if (currentAngle_ >= 2.0f * std::numbers::pi_v<float>) {
            currentAngle_ -= 2.0f * std::numbers::pi_v<float>;  // 角度が2πを超えないように調整
        }
        elapsedTime_ = 0.0f;
    }

    numInstance_ = 0;

    // パーティクルの更新
    for (auto it = particles_.begin(); it != particles_.end();) {
        if (it->currentTime >= it->lifeTime) {
            it = particles_.erase(it);  // 寿命が尽きたパーティクルを削除
            continue;
        }

        // Y方向に移動
        it->transform.translate.y += verticalSpeed_ * kDeltaTime_;

        // 時間とカラーの更新
        it->currentTime += kDeltaTime_;
        float colorShift = it->currentTime / it->lifeTime;
        it->color = { 1.0f, 0.8f - (0.8f * colorShift), 0.5f + (0.5f * colorShift), 1.0f - colorShift };

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
void SpiralEmitter::Draw(BlendMode mode) {
    if (numInstance_ > 0) {
        particle_->Draw(numInstance_, mode);
    }
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void SpiralEmitter::SetPosition(const Vector3& posititon) { transform_.translate = posititon; }

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void SpiralEmitter::UpdateImGui() {
    ImGui::DragFloat("UpSpeed", &verticalSpeed_, 0.01f);
    ImGui::DragFloat("radius", &spiralRadius_, 0.01f);
    ImGui::DragFloat("maxLifeTime", &maxLifetime_, 0.01f);
    ImGui::DragFloat("Interval", &emitInterval_, 0.01f);
    ImGui::DragFloat("rotationSpeed", &rotationSpeed_, 0.01f);
    ImGui::DragFloat("spiralRadiusMax_", &spiralRadiusMax_, 0.01f);
    ImGui::DragFloat("spiralRadiusMin_", &spiralRadiusMin_, 0.01f);
    ImGui::DragFloat("spiralRadiusSpeed_", &spiralRadiusSpeed_, 0.01f);
}

///-------------------------------------------/// 
/// 作成ロジック
///-------------------------------------------///
ParticleData SpiralEmitter::CreateParticle(const Vector3& center, float angle) {
    std::uniform_real_distribution<float> distLifetime(1.0f, maxLifetime_);

    ParticleData particle;
    particle.transform.scale = { 0.1f, 0.1f, 0.1f };  // パーティクルの初期サイズ
    particle.transform.rotate = { 0.0f, 0.0f, 0.0f };

    // 円周上の位置を計算
    particle.transform.translate.x = center.x + spiralRadius_ * std::cos(angle);
    particle.transform.translate.z = center.z + spiralRadius_ * std::sin(angle);
    particle.transform.translate.y = center.y;  // 初期のY座標は中心の高さ

    particle.velocity = { 0.0f, verticalSpeed_, 0.0f };  // 上方向の速度
    particle.color = { 1.0f, 1.0f, 0.5f, 1.0f };         // 初期色
    particle.lifeTime = distLifetime(randomEngine_);     // ランダムな寿命
    particle.currentTime = 0.0f;

    return particle;
}
