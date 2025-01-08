#include "Goal.h"
#include "Game/3d/Camera.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Goal::~Goal() { model_.reset(); }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Goal::Initialze(const std::string & modelName, const Vector3 & pos, Camera* camera) {
	model_ = std::make_unique<Model>();
	model_->Initialize(modelName);
	model_->SetPosition({pos.x, pos.y + 4.0f, pos.z});
	model_->SetCamera(camera);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Goal::Update(Camera * camera) {
	model_->SetCamera(camera);
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Goal::Draw() {
	model_->Draw();
}
