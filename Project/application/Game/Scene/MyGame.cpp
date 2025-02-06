#include "MyGame.h"
// シーンファクトリー
#include "Engine/Scene/SceneFactory.h"
#include "Engine/Service/Loader.h"

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

	/// ===読み込み処理=== ///
	// 音声データの読み込み
	Loader::LoadWave("fanfare", "./Resource/BGM/fanfare.wav");
	// MP3を読み込むとものすごく重い
	Loader::LoadMP3("clear", "./Resource/BGM/clear.mp3");

	// テクスチャの読み込み
	Loader::LoadTexture("uvChecker", "./Resource/uvChecker.png");
	Loader::LoadTexture("monsterBall", "./Resource/monsterBall.png");

	// モデルの読み込み
	Loader::LoadModel("GlTF", ModelFileType::GLTF); // GLTFファイルを読み込むときはModelFileTypeで選択しなければいけない
	Loader::LoadModel("MonsterBall");
	Loader::LoadModel("terrain");
	Loader::LoadModel("axis");
	Loader::LoadModel("plane");
	Loader::LoadModel("Particle");

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