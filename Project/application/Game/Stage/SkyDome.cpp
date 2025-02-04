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
void SkyDome::Initialize(const std::string & filename) {

	model_ = std::make_unique<Model>();
	model_->Initialize(filename);
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
