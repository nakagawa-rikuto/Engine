#include "GameCharacter.h"
// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

/// ===テンプレート候補=== ///
template class GameCharacter<OBBCollider>;
template class GameCharacter<SphereCollider>;

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
template<typename TCollider>
GameCharacter<TCollider>::~GameCharacter() {
	this->object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Initialize() {
	/// ===BaseInfoの初期化設定=== ///
	baseInfo_.velocity = { 0.0f, 0.0f, 0.0f };
	baseInfo_.deltaTIme = 1.0f / 60.0f;
	baseInfo_.isDead = false;

	/// ===OBBCollider=== ///
	TCollider::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Update() {
	/// ===位置の更新=== ///
	this->transform_.translate += baseInfo_.velocity;

	/// ===OBBCollider=== ///
	TCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Draw(BlendMode mode) {
	/// ===OBBCollider=== ///
	TCollider::Draw(mode);
}

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Information() {
#ifdef USE_IMGUI
	ImGui::DragFloat3("Translate", &this->transform_.translate.x, 0.1f);
	ImGui::DragFloat4("Rotate", &this->transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("Scale", &this->transform_.scale.x, 0.1f);
	ImGui::ColorEdit4("Color", &this->color_.x);
	ImGui::DragFloat3("Velocity", &baseInfo_.velocity.x, 0.1f);
	ImGui::DragFloat("DeltaTime", &baseInfo_.deltaTIme, 0.01f, 0.0f, 1.0f);
	ImGui::Checkbox("IsDead", &baseInfo_.isDead);
	TCollider::Info();
#endif // USE_IMGUI
}