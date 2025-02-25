#include "SkyDome.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
SkyDome::~SkyDome() {
	model_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SkyDome::Initialize(const std::string modelname) {
	model_ = std::make_unique<Model>();
	model_->Initialize(modelname);
	model_->SetScale({ 10.0f, 10.0f, 10.0f });
	model_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void SkyDome::Update(Camera* camera) {
	model_->SetCamera(camera);
	model_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SkyDome::Draw() {
	model_->Draw();
}
