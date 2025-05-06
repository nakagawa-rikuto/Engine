#pragma once
/// ===Include=== ///
#include <memory>

/// ===前方宣言=== ///
class WinApp;
class DXCommon;
class SRVManager;
class RTVManager;
class DSVManager;
class AudioManager;
class TextureManager;
class ModelManager;
class PipelineManager;
class AnimationManager;
class CSVManager;
class OffScreenRenderer;
class Keyboard;
class Mouse;
class Controller;

struct ServiceRegister {
	WinApp* winApp = nullptr;
	DXCommon* dxCommon = nullptr;
	SRVManager* srvManager = nullptr;
	RTVManager* rtvManager = nullptr;
	DSVManager* dsvManager = nullptr;
	PipelineManager* pipelineManager = nullptr;
	TextureManager* textureManager = nullptr;
	ModelManager* modelManager = nullptr;
	AnimationManager* animationManager = nullptr;
	AudioManager* audioManager = nullptr;
	CSVManager* csvManager = nullptr;
	OffScreenRenderer* offScreenRenderer = nullptr;
	Keyboard* keyboard = nullptr;
	Mouse* mouse = nullptr;
	Controller* controller = nullptr;
};

///=====================================================/// 
/// サービスロケータ
///=====================================================///
class ServiceLocator {
public:
	// 全てのサービスを提供
	static void ProvideAll(const ServiceRegister& registry);
	// 全てのサービスを終了
	static void Finalize();

	// WinApp
	static WinApp* GetWinApp();
	// DXCommon
	static DXCommon* GetDXCommon();
	// SRVManager
	static SRVManager* GetSRVManager();
	// RTVManager 
	static RTVManager* GetRTVManager();
	// DSVManager
	static DSVManager* GetDSVManager();
	// PipelineManager
	static PipelineManager* GetPipelineManager();
	// TextureManager
	static TextureManager* GetTextureManager();
	// ModelManager
	static ModelManager* GetModelManager();
	// AnimationManager
	static AnimationManager* GetAnimationManager();
	// CSVManager
	static CSVManager* GetCSVManager();
	// AudioManager
	static AudioManager* GetAudioManager();
	// OffScreenRenderer
	static OffScreenRenderer* GetOffScreenRenderer();
	// Input (Keyboard, Mouse, Controller)
	static Keyboard* GetKeyboard();
	static Mouse* GetMouse();
	static Controller* GetController();

private:
	static inline WinApp* winApp_ = nullptr;
	static inline DXCommon* dxCommon_ = nullptr;
	static inline SRVManager* srvManager_ = nullptr;
	static inline RTVManager* rtvManager_ = nullptr;
	static inline DSVManager* dsvManager_ = nullptr;
	static inline PipelineManager* pipelineManager_ = nullptr;
	static inline TextureManager* textureManager_ = nullptr;
	static inline ModelManager* modelManager_ = nullptr;
	static inline AnimationManager* animationManager_ = nullptr;
	static inline CSVManager* csvManager_ = nullptr;
	static inline AudioManager* audioManager_ = nullptr;
	static inline OffScreenRenderer* offScreenRenderer_ = nullptr;
	static inline Keyboard* keyboard_ = nullptr;
	static inline Mouse* mouse_ = nullptr;
	static inline Controller* controller_ = nullptr;
};

