#pragma once
/// ===include=== ///
// C++
#include <string>

// DirectXTex
#include "DirectXTex.h"

// Original
#include "Base/ComPtr/ComPtr.h"
#include "Base/Data/CBufferData.h"

/// ===前方宣言=== ///
class DXCommon;

///=====================================================/// 
/// テクスチャマネージャ
///=====================================================///
class TextureManager {

public:/// ===基本的な関数=== ///
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DXCommon* dxCommon);

public:/// ===Getter=== ///

	/// <summary>
	/// テクスチャ番号からGPUハンドルを取得
	/// </summary>
	/// <param name="textureIndex"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(uint32_t textureIndex);

	/// <summary>
	/// SRVインデックス開始番号
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

public:/// ===Variables(変数)=== ///

	/// ===ImGui対応=== ///
	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop_;

public:/// ===Functions(関数)=== ///

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadTexture(const std::string& filePath);

	/// <summary>
	/// データを転送する
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="mipImages"></param>
	void UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

private:/// ===Variables(変数)=== ///
	
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = default;

	DXCommon* dxCommon_ = nullptr;

	/// ===テクスチャデータの構造体=== ///
	// テクスチャ1枚分のデータ
	struct TextureData {
		std::string filePath;
		DirectX::TexMetadata metadata;
		ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	/// ===テクスチャデータコンテナ=== ///
	// テクスチャデータ
	std::vector<TextureData> textureDatas_;

private:/// ===Functions(関数)=== ///

	/// <summary>
	/// TextureResourceの作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="metadata"></param>
	/// <returns></returns>
	ComPtr<ID3D12Resource> CreateTextureResource( const DirectX::TexMetadata& metadata);
};

