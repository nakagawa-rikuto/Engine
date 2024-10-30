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
	
	// コンストラクタ
	TextureManager() = default;
	// デストラクタ
	~TextureManager() = default;

	// 初期化
	void Initialize(DXCommon* dxCommon);

public:/// ===Getter=== ///

	// テクスチャ番号からGPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(uint32_t textureIndex);
	// SRVインデックス開始番号
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
	// ミップマップの生成
	DirectX::ScratchImage Load(const std::string& filePath);
	// TextureResourceの作成
	ComPtr<ID3D12Resource> CreateTextureResource( const DirectX::TexMetadata& metadata);
	// データを転送する
	void UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);
};

