#include "ServiceLocator.h"
// c++
#include <cassert>

///-------------------------------------------/// 
/// 全てのサービスを提供
///-------------------------------------------///
void ServiceLocator::ProvideAll(const ServiceRegister& registry) {
	/// ===nullチェック=== ///
	// WinApp
	assert(registry.winApp);
	// DXCommon
	assert(registry.dxCommon);
	// SRV, RTV, DSV
	assert(registry.srvManager);
	assert(registry.dsvManager);
	assert(registry.rtvManager);
	// PiplineManager
	assert(registry.pipelineManager);
	// TextureManager
	assert(registry.textureManager);
	// ModelManager
	assert(registry.modelManager);
	// AnimationManager
	assert(registry.animationManager);
	// AudioManager
	assert(registry.audioManager);
	// CSVManager
	assert(registry.csvManager);
	// OffScreenRenderer
	assert(registry.offScreenRenderer);
	// LineObject3D
	assert(registry.lineObject3D);
	// Input
	assert(registry.keyboard);
	assert(registry.mouse);
	assert(registry.controller);
	// CameraManager
	assert(registry.cameraManager);
	// ParticleManager
	assert(registry.particleManager);

	// WinApp
	winApp_ = registry.winApp;
	// DXCommon
	dxCommon_ = registry.dxCommon;
	// SRV, RTV, DSV
	srvManager_ = registry.srvManager;
	rtvManager_ = registry.rtvManager;
	dsvManager_ = registry.dsvManager;
	// Pipline
	pipelineManager_ = registry.pipelineManager;
	// Texture
	textureManager_ = registry.textureManager;
	// Model
	modelManager_ = registry.modelManager;
	// Animation
	animationManager_ = registry.animationManager;
	// Audio
	audioManager_ = registry.audioManager;
	// CSV
	csvManager_ = registry.csvManager;
	// OffScreen
	offScreenRenderer_ = registry.offScreenRenderer;
	// LineObject
	lineObject3D_ = registry.lineObject3D;
	// Input
	keyboard_ = registry.keyboard;
	mouse_ = registry.mouse;
	controller_ = registry.controller;
	// CameraManager
	cameraManager_ = registry.cameraManager;
	// ParticleManager
	particleManager_ = registry.particleManager;
}

///-------------------------------------------/// 
/// 全てのサービスを終了
///-------------------------------------------///
void ServiceLocator::Finalize() {
	cameraManager_ = nullptr;
	particleManager_ = nullptr;
	controller_ = nullptr;
	mouse_ = nullptr;
	keyboard_ = nullptr;
	csvManager_ = nullptr;
	audioManager_ = nullptr;
	lineObject3D_ = nullptr;
	offScreenRenderer_ = nullptr;
	animationManager_ = nullptr;
	modelManager_ = nullptr;
	textureManager_ = nullptr;
	pipelineManager_ = nullptr;
	dsvManager_ = nullptr;
	rtvManager_ = nullptr;
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
/// LineObject3D
///-------------------------------------------///
LineObject3D* ServiceLocator::GetLineObject3D() { return lineObject3D_; }

///-------------------------------------------/// 
/// OffScreenRenderer
///-------------------------------------------///
OffScreenRenderer* ServiceLocator::GetOffScreenRenderer() { return offScreenRenderer_; }

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

///-------------------------------------------/// 
/// CameraManager
///-------------------------------------------///
CameraManager* ServiceLocator::GetCameraManager() { return cameraManager_; }

///-------------------------------------------/// 
/// ParticleManager
///-------------------------------------------///
ParticleManager* ServiceLocator::GetParticleManager() { return particleManager_; }