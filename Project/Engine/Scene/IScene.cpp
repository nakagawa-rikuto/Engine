#include "IScene.h"
// Service
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/System/Service/CameraService.h"
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/Loader.h"
// Object3d
#include "Engine/Graphics/3d/Model/Model.h"
// Line
#include "Engine/Graphics/3d/Line/LineObject3D.h"
// Math
#include "Math/sMath.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
IScene::~IScene() {
	defaultCamera_.reset();

	/*if (IsLevelLoaded_) {
		for (auto obj : objects_) {
			delete obj;
		}
		objects_.clear();

		for (auto& [key, model] : models_) {
			delete model;
		}
		models_.clear();
	}*/
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void IScene::Initialize() {

	/// ===デフォルトカメラの生成=== ///
	defaultCamera_ = std::make_shared<Camera>();
	defaultCamera_->Initialize();
	defaultCamera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	defaultCamera_->SetRotate({ 0.0f, 0.0f, 0.0f });

	// デフォルトカメラの設定
	CameraService::Add("Default", defaultCamera_);
	CameraService::SetActiveCamera("Default");
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void IScene::Update() {

	// レベルが読み込まれている場合
	/*if (IsLevelLoaded_) {
		for (Model* model : objects_) {
			model->Update();
		}
	}*/

	// Line更新
	ServiceLocator::GetLineObject3D()->SetCamera(CameraService::GetActiveCamera().get());
	ServiceLocator::GetLineObject3D()->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void IScene::Draw() {

	// レベルが読み込まれている場合
	/*if (IsLevelLoaded_) {
		for (Model* model : objects_) {
			model->Draw(BlendMode::kBlendModeNone);
		}
	}*/

	// Lineの描画
	ServiceLocator::GetLineObject3D()->Draw();
}

///-------------------------------------------/// 
/// モデルデータの先読み
///-------------------------------------------///
//void IScene::LoadModelsForLevel(const std::string& file_name) {
//
//	levelData_ = GraphicsResourceGetter::GetLevelData(file_name);
//
//	for (const auto& obj : levelData_->objects) {
//		const std::string& fileName = obj.fileName;
//		Loader::LoadModel(fileName, fileName);
//	}
//
//	// フラグをtrueに設定
//	IsLevelLoaded_ = true;
//}

///-------------------------------------------/// 
/// レベルオブジェクトの配置
///-------------------------------------------///
//void IScene::PlaceLevelObjects() {
//
//	// 早期リターン
//	if (!IsLevelLoaded_) {
//		return;
//	}
//
//
//	std::vector<Model*> objects; // 配置済みオブジェクト格納先
//
//	for (const auto& objectData : levelData_->objects) {
//		Model* model = nullptr;
//
//		// モデルキャッシュから取得
//		auto it = models_.find(objectData.fileName);
//		if (it != models_.end()) {
//			model = it->second;
//		}
//
//		// モデルから新しいインスタンスを生成
//		Model* newObject = new Model();
//		newObject->Initialize(objectData.fileName);
//		newObject->SetTranslate(objectData.translation);
//		newObject->SetRotate(Math::QuaternionFromVector(objectData.rotation));
//		newObject->SetScale(objectData.scaling);
//
//		objects.push_back(newObject);
//	}
//
//	// 一括格納
//	objects_ = std::move(objects);
//}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void IScene::SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
