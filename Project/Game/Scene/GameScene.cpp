#include "Game/Scene/GameScene.h"
// SceneManager
#include "Game/Manager/SceneManager.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

    const std::string& BlockModel = "Block";
    Mii::LoadModel(BlockModel);

    // 配置基準点と間隔
    Vector3 basePoint(0.0f, 0.0f, 0.0f);
    float offset = 10.0f; // 各モデルの間隔

    // モデルの初期化と配置
    for (size_t i = 0; i < 3; ++i) {
        auto model = std::make_shared<Model>();
        model->Initialize(BlockModel);

        // 近接した位置に配置 (X 軸方向に並べる)
        Vector3 position(
            basePoint.x + i * offset, // X方向にオフセット
            basePoint.y,              // Yは基準点そのまま
            basePoint.z               // Zも基準点そのまま
        );

        // モデルに位置を設定
        model->SetPosition(position);

        // モデルをリストに追加
        block_.push_back(model);
    }

    //Camera
    camera3D_ = std::make_shared<Camera>();
    camera3D_->Initialize();
    camera3D_->SetTranslate({ 0.0f, 0.0f, -10.0f });
    camera3D_->SetRotate({ 0.0f, 0.0f, 0.0f });

    camera2D_ = std::make_shared<Camera>();
    camera2D_->Initialize();
    camera2D_->SetTranslate({ 0.0f, 0.0f, -50.0f });
    camera2D_->SetRotate({ 0.0f, 0.0f, 0.0f });

    cameraManager_->Add("3D", camera3D_);
    cameraManager_->Add("2D", camera2D_);

    cameraManager_->SetActiveCamera("3D");
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef _DEBUG
	ImGui::Begin("GameScene");

    ImGui::Begin("Camera");
    ImGui::Checkbox("Flag", &SetCamera);
    ImGui::DragFloat3("Translate", &cameraPos.x, 0.1f);
    ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.1f);
    ImGui::End();

	ImGui::End();
#endif // DEBUG

    if (SetCamera) {
        cameraManager_->SetActiveCamera("3D");
    } else {
        cameraManager_->SetActiveCamera("2D");
    }

    for (const auto& model : block_) {
        if (model) {
            //model->SetCamera(cameraManager_->GetActiveCamera().get());
        }
    }
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
    // block_ 内の各モデルの Draw を呼び出す
    for (const auto& model : block_) {
        if (model) { // nullptr チェック
            model->Draw();
        }
    }
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}