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

    srvManager_ = registry.srvManager;
    audioManager_ = registry.audioManager;
    textureManager_ = registry.textureManager;
    modelManager_ = registry.modelManager;
    pipelineManager_ = registry.pipelineManager;
    animationManager_ = registry.animationManager;
    csvManager_ = registry.csvManager;
    dxCommon_ = registry.dxCommon;
    winApp_ = registry.winApp;
    keyboard_ = registry.keyboard;
    mouse_ = registry.mouse;
    controller_ = registry.controller;
}

///-------------------------------------------/// 
/// SRVManager
///-------------------------------------------///
SRVManager* ServiceLocator::GetSRVManager() { return srvManager_; }

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
