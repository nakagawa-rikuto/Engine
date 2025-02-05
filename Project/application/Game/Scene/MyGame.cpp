#include "MyGame.h"
// シーンファクトリー
#include "Engine/Scene/SceneFactory.h"
#include "Engine/Core/Mii.h"

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

	/// ===Loadの生成=== ///
	Loader_ = std::make_unique<AssetLoader>();
	Loader_->Inititalze(Mii::GetTextureManager(), Mii::GetModelManager(), Mii::GetAudioManager(), Mii::GetCSVManager());
	
	/// ===Spriteの読み込み=== ///
	// TitleScene
	const std::string& titleTexture = "Resource/title.png";
	Loader_->LoadTexture(titleTexture);
	// bg
	const std::string& bgSprite = "Resource/backGround.png";
	Loader_->LoadTexture(bgSprite);
	// Tutorial
	const std::string& tutorialSprite = "Resource/Tutorial/Tutorial.png";
	const std::string& tutorialArrowSprite = "Resource/Tutorial/Arrow.png";
	Loader_->LoadTexture(tutorialSprite);
	Loader_->LoadTexture(tutorialArrowSprite);
	//Situation
	const std::string& Retry = "Resource/Scene/Retry.png";
	const std::string& Select = "Resource/Scene/Select.png";
	const std::string& TitleSelect = "Resource/Scene/TitleSelect.png";
	const std::string& Clear = "Resource/gameClear.png";
	const std::string& GameOver = "Resource/gameOver.png";
	Loader_->LoadTexture(Select);
	Loader_->LoadTexture(Retry);
	Loader_->LoadTexture(TitleSelect);
	Loader_->LoadTexture(Clear);
	Loader_->LoadTexture(GameOver);

	/// ===モデルの読み込み=== ///
	// GameScene
	const std::string& CardModel1 = "Card1";
	const std::string& CardModel2 = "Card2";
	const std::string& CardModel3 = "Card3";
	const std::string& CardModel4 = "Card4";
	Loader_->LoadModel(CardModel1);
	Loader_->LoadModel(CardModel2);
	Loader_->LoadModel(CardModel3);
	Loader_->LoadModel(CardModel4);
	// パーティクル
	const std::string& particle = "Particle";
	Loader_->LoadModel(particle);

	// Audio
	// 音声データの読み込み
	Loader_->LoadWave("fanfare", "./Resource/BGM/fanfare.wav");
	Loader_->LoadWave("flipCard", "./Resource/SE/flipCard.wav");
	Loader_->LoadWave("clock", "./Resource/BGM/clock.wav");
	Loader_->LoadWave("GObgm", "./Resource/BGM/GameOver.wav");
	// MP3を読み込むとものすごく重い
	Loader_->LoadMP3("clear", "./Resource/BGM/clear.mp3");
	Loader_->LoadMP3("GCbgm", "./Resource/BGM/GameClear.mp3");
	Loader_->LoadMP3("GamePlay", "./Resource/BGM/GamePlay.mp3");
	// シーンファクトリーの生成
	sceneFactory_ = std::make_unique<SceneFactory>();
	// シーンマネージャの初期化
	sceneManager_ = std::make_unique<SceneManager>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());
	sceneManager_->ChangeScene("Title");
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