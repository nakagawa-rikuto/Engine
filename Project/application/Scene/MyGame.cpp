#include "MyGame.h"
// シーンファクトリー
#include "Engine/Scene/SceneFactory.h"
#include "Engine/System/Service/Loader.h"
#include "Engine/Core/Logger.h"
// c++
#include <iostream>
#include <thread>
#include <chrono>

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
MyGame::MyGame() = default;
MyGame::~MyGame() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void MyGame::Initialize(const wchar_t* title) {
	// 基底クラスの初期化
	Framework::Initialize(title);
	
	/// ===時間計測=== ///
	auto start = std::chrono::high_resolution_clock::now();

	/// ===読み込み処理=== ///
	std::vector<std::thread> threads;

	threads.emplace_back([this] { LoadAudio(); });
	threads.emplace_back([this] { LoadTexture(); });
	threads.emplace_back([this] { LoadModel(); });
	threads.emplace_back([this] { LoadAnimation(); });

	// 全スレッドの終了を待機
	for (auto& thread : threads) {
		thread.join();
	}

	// 処理時間を計測（end）
	auto end = std::chrono::high_resolution_clock::now();

	// 経過時間をミリ秒単位で出力
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	Log("time: " + std::to_string(duration.count()) + " ms\n");

	/// ===シーンの作成=== ///
	// シーンファクトリーの生成
	sceneFactory_ = std::make_unique<SceneFactory>();
	// シーンマネージャの初期化
	sceneManager_ = std::make_unique<SceneManager>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());
	sceneManager_->ChangeScene(SceneType::Debug);
}
 
///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void MyGame::Finalize() {
	// シーンの解放処理
	sceneManager_.reset();
	sceneFactory_.reset();
	// 基底クラスの終了処理
	Framework::Finalize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void MyGame::Update() {
	// 基底クラスの更新処理
	Framework::Update();
	// シーンの更新
	sceneManager_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void MyGame::Draw() {
	// 描画前処理
	Framework::PreDraw();
	// シーンの描画
	sceneManager_->Draw();
	// 描画後処理
	Framework::PostDraw();
}

///-------------------------------------------/// 
/// 読み込み関数
///-------------------------------------------///
// 音
void MyGame::LoadAudio() {
	// Wave
	Loader::LoadWave("fanfare", "fanfare.wav");
	// MP3
	Loader::LoadMP3("clear", "clear.mp3");
}
// テクスチャ
void MyGame::LoadTexture() {
	Loader::LoadTexture("uvChecker", "uvChecker.png");
	Loader::LoadTexture("monsterBall", "monsterBall.png");
}
// モデル
void MyGame::LoadModel() {
	Loader::LoadModel("GlTF", "GlTF.gltf");
	Loader::LoadModel("MonsterBall", "MonsterBall.obj");
	Loader::LoadModel("terrain", "terrain.obj");
	Loader::LoadModel("axis", "axis.obj");
	Loader::LoadModel("plane", "plane.obj");
	Loader::LoadModel("Particle", "Particle.obj");
}
// アニメーション
void MyGame::LoadAnimation() {
	Loader::LoadAnimation("AnimatedCube", "AnimatedCube.gltf");
	Loader::LoadAnimation("simpleSkin", "simpleSkin.gltf");
	Loader::LoadAnimation("human", "sneakWalk.gltf");
}
