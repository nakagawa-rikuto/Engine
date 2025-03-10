
#include "Ground.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Ground::~Ground() {
	model_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Ground::Initialize(const std::string modelname) {
	model_ = std::make_unique<Model>();
	model_->Initialize(modelname);
	model_->SetPosition(Vector3(0.0f, -1.0f, 0.0f));
	model_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Ground::Update(Camera* camera) {
	model_->SetCamera(camera);
	model_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Ground::Draw() {
	model_->Draw(BlendMode::kBlendModeNone);
}
