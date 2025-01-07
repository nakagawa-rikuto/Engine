#include "Game/Scene/GameScene.h"
// SceneManager
#include "Game/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"
#include "Math/sMath.h"

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

    /// ===読み込み=== ///
    // Blockモデルの読み込み
    const std::string& BlockModel = "Block";
    Loader_->LoadModel(BlockModel);
    // Playerモデルの読み込み
    const std::string& PlayerModel = "Player";
    Loader_->LoadModel(PlayerModel);

    /// ===Player関連=== ///
    player_ = std::make_unique<Player>();
    player_->Inititalze(PlayerModel);

    /// ===Block関連=== ///
     // 配置基準点と間隔
    Vector3 basePoint(0.0f, 0.0f, 0.0f);
    float offset = 10.0f; // 各モデルの間隔

    // Blockの配置
    for (size_t i = 0; i < 10; ++i) {
        auto block = std::make_shared<Block>();

        //// 近接した位置に配置 (X 軸方向に並べる)
        Vector3 position(
            basePoint.x + i * block->GetSize().x * 0.5f,
            basePoint.y,
            basePoint.z + i * offset
        );

        if (i == 8) {
            position.x = basePoint.x + i * block->GetSize().x * 0.5f;
            position.y = basePoint.y;
            position.z = basePoint.z + (i - 1) * offset + block->GetSize().z * 0.5f;
        }

        // 最後のモデルだけ Z 軸方向に +5.0f
        if (i == 9) {
            position.x = basePoint.x + (i - 1) * block->GetSize().x * 0.5f;
            position.y = basePoint.y;
            position.z = basePoint.z + (i - 2) * offset;
        }

        // 簡単デバッグ用
        /*Vector3 position(
            basePoint.x + i * block->GetSize().x * 0.5f,
            basePoint.y,
            basePoint.z
        );*/

        block->Initialze(BlockModel, position); // 初期化を呼び出す
        blocks_.push_back(block);  // モデルをリストに追加
    }

    /// ===カメラ関連=== ///
    // カメラのTransform情報を書き込む(最初は3D)
    cameraPos_ = { player_->GetPos().x, player_->GetPos().y + 7.0f, player_->GetPos().z - 40.0f};
    cameraRotate_ = { 0.05f, 0.0f, 0.0f };
    cameraScale_ = { 1.0f, 1.0f, 1.0f };

    //Camera
    camera_ = std::make_shared<Camera>();
    camera_->Initialize();
    camera_->SetTranslate(cameraPos_);
    camera_->SetRotate(cameraRotate_);
    camera_->SetScale(cameraScale_);

    // カメラの追加
    cameraManager_->Add("3D", camera_);
    // アクティブカメラのセット
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
    ImGui::DragFloat3("Translate", &cameraPos_.x, 0.01f);
    ImGui::DragFloat3("Rotate", &cameraRotate_.x, 0.01f);
    ImGui::DragFloat3("Scale", &cameraScale_.x, 0.01f);
    ImGui::End();

	ImGui::End();
#endif // DEBUG

    /// ===モードの切り替え=== ///
    if (Mii::TriggerKey(DIK_Q)) {
        if (Mode3D_) {
            Mode3D_ = false;
            cameraPos_ = { -8.5f, -9.5f, 0.0f };
            cameraRotate_ = { 0.0f, 0.0f, 0.0f };
            cameraScale_ = { 1.0f, 0.5f, 1.0f };
        } else {
            SwithcTo3DMode();
            Mode3D_ = true;
            cameraPos_ = { player_->GetPos().x, player_->GetPos().y + 7.0f, player_->GetPos().z - 40.0f };
            cameraRotate_ = { 0.05f, 0.0f, 0.0f };
            cameraScale_ = { 1.0f, 1.0f, 1.0f };
        }
    }
    // カメラの切り替え
    if (Mode3D_) {
        camera_->SwitchTo3DMode();
        cameraPos_ = { player_->GetPos().x, player_->GetPos().y + 7.0f, player_->GetPos().z - 40.0f };
        // 衝突判定(3D)
        IsCollisison3D();
    } else {
        camera_->SwitchTo2DMode();
        // 衝突判定(2D)
        IsCollisison2D();
    }

    // Playerの更新
    player_->Update(cameraManager_->GetActiveCamera().get(), Mode3D_);
    // Blockの更新
    for (const auto& block : blocks_) {
        if (block) {
            block->Update(cameraManager_->GetActiveCamera().get());
        }
    }

    // カメラセット
    camera_->SetTranslate(cameraPos_);
    camera_->SetRotate(cameraRotate_);
    camera_->SetScale(cameraScale_);
    cameraManager_->UpdateAllCameras();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
    // Blockの描画
    for (const auto& block : blocks_) {
        if (block) { // nullptr チェック
            block->Draw();
        }
    }
    // Playerの描画
    player_->Draw();

#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
// 2D
void GameScene::IsCollisison2D() {
    for (const auto& block : blocks_) {
        for (size_t i = 0; i < blocks_.size(); ++i) {
            const Vector2 blockCenter = Vector2(blocks_[i]->GetPos().x, blocks_[i]->GetPos().y); // 中心座標
            const Vector2 blockHalfSize = Vector2(block->GetSize().x * 0.5f, block->GetSize().y * 0.5f); // 矩形の半分のサイズ
            const Vector2 playerCenter = Vector2(player_->GetPos().x, player_->GetPos().y);

            // 衝突判定
            if (IsCircleIntersectingRectangle(playerCenter, player_->GetRadius(), blockCenter, blockHalfSize)) {
                // 衝突が発生した場合,Playerに通知
                player_->OnCollision(blocks_[i].get());
                player_->SetBlockPosition(blocks_[i].get());
            } else {
                player_->NotCollisision();
            }
        }
    }
}
// 3D
void GameScene::IsCollisison3D() {
    for (const auto& block : blocks_) {
        for (size_t i = 0; i < blocks_.size(); ++i) {
            // 各Modelの位置とサイズを取得
            const Vector3 blockMin = block->GetPos() - (block->GetSize() * 0.5f);
            const Vector3 blockMax = block->GetPos() + (block->GetSize() * 0.5f);

            // 衝突判定
            if (IsSphereIntersectingAABB(player_->GetPos(), player_->GetRadius(), blockMin, blockMax)) {
                // 衝突が発生した場合,Playerに通知
                player_->OnCollision(blocks_[i].get());
            } else {
                player_->NotCollisision();
            }
        }
    }
}

///-------------------------------------------/// 
/// 2Dから3Dに切り替えるときの座標変換
///-------------------------------------------///
void GameScene::SwithcTo3DMode() {
   /* if (player_->GetIsCollision()) {
        
    }*/
    player_->SetPosititonZ(player_->GetBlockPos().z);
}
