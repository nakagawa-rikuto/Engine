#include "ServiceLocator.h"
// c++
#include <cassert>

///-------------------------------------------/// 
/// 全てのサービスを提供
///-------------------------------------------///
void ServiceLocator::ProvideAll(const ServiceRegister& registry) {
    // nullチェック
    assert(registry.srvManager);
    assert(registry.audioManager);
    assert(registry.animationManager);
    assert(registry.textureManager);
    assert(registry.modelManager);
    assert(registry.pipelineManager);
    assert(registry.animationManager);
    assert(registry.csvManager);
    assert(registry.dxCommon);
    assert(registry.winApp);
    assert(registry.keyboard);
    assert(registry.mouse);
    assert(registry.controller);

    dxCommon_ = registry.dxCommon;
    winApp_ = registry.winApp;
    srvManager_ = registry.srvManager;
    pipelineManager_ = registry.pipelineManager;
    textureManager_ = registry.textureManager;
    modelManager_ = registry.modelManager;
    animationManager_ = registry.animationManager;
    audioManager_ = registry.audioManager;
    csvManager_ = registry.csvManager;
    keyboard_ = registry.keyboard;
    mouse_ = registry.mouse;
    controller_ = registry.controller;
}

///-------------------------------------------/// 
/// 全てのサービスを終了
///-------------------------------------------///
void ServiceLocator::Finalize() {
    controller_ = nullptr;
    mouse_ = nullptr;
    keyboard_ = nullptr;
    csvManager_ = nullptr;
    audioManager_ = nullptr;
    animationManager_ = nullptr;
    modelManager_ = nullptr;
    textureManager_ = nullptr;
    pipelineManager_ = nullptr;
    srvManager_ = nullptr;
    winApp_ = nullptr;
    dxCommon_ = nullptr;
}

///-------------------------------------------/// 
/// SRVManager
///-------------------------------------------///
SRVManager* ServiceLocator::GetSRVManager() { return srvManager_; }

///-------------------------------------------/// 
/// RTVManager
///-------------------------------------------///
RTVManager* ServiceLocator::GetRTVManager() { return rtvManager_; }

///-------------------------------------------/// 
/// DSVManager
///-------------------------------------------///
DSVManager* ServiceLocator::GetDSVManager() { return dsvManager_; }

///-------------------------------------------/// 
/// AudioManager
///-------------------------------------------///
AudioManager* ServiceLocator::GetAudioManager() { return audioManager_; }

///-------------------------------------------/// 
/// TextureManager
///-------------------------------------------///
TextureManager* ServiceLocator::GetTextureManager() { return textureManager_; }

///-------------------------------------------/// 
/// ModelManager
///-------------------------------------------///
ModelManager* ServiceLocator::GetModelManager() { return modelManager_; }

///-------------------------------------------/// 
/// AnimationManager
///-------------------------------------------///
AnimationManager* ServiceLocator::GetAnimationManager() { return animationManager_; }

///-------------------------------------------/// 
/// CSVManager
///-------------------------------------------///
CSVManager* ServiceLocator::GetCSVManager() { return csvManager_; }

///-------------------------------------------/// 
/// PipelineManager
///-------------------------------------------///
PipelineManager* ServiceLocator::GetPipelineManager() { return pipelineManager_; }

///-------------------------------------------/// 
/// DXCommon
///-------------------------------------------///
DXCommon* ServiceLocator::GetDXCommon() { return dxCommon_; }

///-------------------------------------------/// 
/// WinApp
///-------------------------------------------///
WinApp* ServiceLocator::GetWinApp() { return winApp_; }

///-------------------------------------------/// 
/// Input
///-------------------------------------------///
Keyboard* ServiceLocator::GetKeyboard() { return keyboard_; }
Mouse* ServiceLocator::GetMouse() { return mouse_; }
Controller* ServiceLocator::GetController() { return controller_; }
