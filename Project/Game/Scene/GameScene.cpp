#include "Game/Scene/GameScene.h"
// Engine
#include "Engine/Core/System.h"
#include "Engine/Core/DXCommon.h"
#include "Engine/Input/Input.h"
// Math
#include "Math/sMath.h"

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
GameScene::GameScene() {}
GameScene::~GameScene() {}

/* ////////////////////////////////////////////////////////////
					　Getter ・　Setter
*/ ////////////////////////////////////////////////////////////

/* ////////////////////////////////////////////////////////////
							初期化
*/ ////////////////////////////////////////////////////////////
void GameScene::Initialize() {
	/// ===デフォルトカメラの初期化=== ///
	//NOTE:後々ISceneに移行
	defaultCamera_ = std::make_unique<Camera>();
	defaultCamera_->Initialize();
	defaultCamera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	defaultCamera_->SetRotate({ 0.0f, 0.0f, 0.0f });
}

/* ////////////////////////////////////////////////////////////
							 更新
*/ ////////////////////////////////////////////////////////////
void GameScene::Update() {}

/* ////////////////////////////////////////////////////////////
							 描画
*/ ////////////////////////////////////////////////////////////
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = System::GetDXCommandList();

#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

/* ////////////////////////////////////////////////////////////
						　メンバ関数
*/ ////////////////////////////////////////////////////////////
