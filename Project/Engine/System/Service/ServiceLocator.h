#pragma once
/// ===Include=== ///
#include <memory>

/// ===前方宣言=== ///
class SRVManager;
class AudioManager;
class TextureManager;
class ModelManager;
class PipelineManager;
class AnimationManager;
class CSVManager;
class DXCommon;
class WinApp;
class Keyboard;
class Mouse;
class Controller;

struct ServiceRegister {
    DXCommon* dxCommon = nullptr;
    WinApp* winApp = nullptr;
    SRVManager* srvManager = nullptr;
    PipelineManager* pipelineManager = nullptr;
    TextureManager* textureManager = nullptr;
    ModelManager* modelManager = nullptr;
    AnimationManager* animationManager = nullptr;
    AudioManager* audioManager = nullptr;
    CSVManager* csvManager = nullptr;
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

    // SRVManager
    static SRVManager* GetSRVManager();
    // AudioManager
    static AudioManager* GetAudioManager();
    // TextureManager
    static TextureManager* GetTextureManager();
    // ModelManager
    static ModelManager* GetModelManager();
    // AnimationManager
    static AnimationManager* GetAnimationManager();
    // CSVManager
    static CSVManager* GetCSVManager();
    // PipelineManager
    static PipelineManager* GetPipelineManager();
    // DXCommon
    static DXCommon* GetDXCommon();
    // WinApp
    static WinApp* GetWinApp();
    // Input (Keyboard, Mouse, Controller)
    static Keyboard* GetKeyboard();
    static Mouse* GetMouse();
    static Controller* GetController();

private:
    static inline SRVManager* srvManager_ = nullptr;
    static inline AudioManager* audioManager_ = nullptr;
    static inline TextureManager* textureManager_ = nullptr;
    static inline ModelManager* modelManager_ = nullptr;
    static inline AnimationManager* animationManager_ = nullptr;
    static inline CSVManager* csvManager_ = nullptr;
    static inline PipelineManager* pipelineManager_ = nullptr;
    static inline DXCommon* dxCommon_ = nullptr;
    static inline WinApp* winApp_ = nullptr;
    static inline Keyboard* keyboard_ = nullptr;
    static inline Mouse* mouse_ = nullptr;
    static inline Controller* controller_ = nullptr;
};

