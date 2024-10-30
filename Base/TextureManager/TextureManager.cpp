#include "TextureManager.h"

#include "Base/DirectXCommon/DXCommon.h"
#include "Base/StringUtility/StringUtility.h"

#include <cassert>

// StringUtility
using namespace StringUtility;

// ImGuiで0番を使用するため、１番から使用
uint32_t TextureManager::kSRVIndexTop_ = 1;

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void TextureManager::Initialize(DXCommon* dxCommon) {

	dxCommon_ = dxCommon;

	// SRVの数と同数
	textureDatas_.reserve(DXCommon::kMaxSRVCount);
}

///-------------------------------------------/// 
/// テクスチャファイルの読み込み	
///-------------------------------------------///
void TextureManager::LoadTexture(const std::string& filePath) {

	// 読み込み済みのテクスチャを検索
	auto it = std::find_if(
		textureDatas_.begin(), textureDatas_.end(), [&](TextureData& data) {
			return data.filePath == filePath;
		});

	// 読み込み済みなら早期リターン
	if (it != textureDatas_.end()) return;

	// 上限チェックとエラーハンドリング
	if (textureDatas_.size() + kSRVIndexTop_ >= DXCommon::kMaxSRVCount) {
		throw std::runtime_error("Exceeded maximum SRV count");
	}

	// テクスチャデータの追加
	textureDatas_.emplace_back();

	// 追加したテクスチャデータの参照を取得
	TextureData& textureData = textureDatas_.back();

	// テクスチャデータの書き込み
	textureData.filePath = filePath;
	DirectX::ScratchImage mipImages = Load(filePath); // ミップマップの作成
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = CreateTextureResource(textureData.metadata);

	// テクスチャを転送
	UploadTextureData(textureData.resource, mipImages);

	// テクスチャデータの要素数番号をSRVのインデックスを計算する
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas_.size() - 1) + kSRVIndexTop_;

	// SRVを作成するDescriptorHeapの場所設定
	textureData.srvHandleCPU = dxCommon_->GetSRVCPUDescriptorHandle(srvIndex);
	textureData.srvHandleGPU = dxCommon_->GetSRVGPUDescriptorHandle(srvIndex);

	// SRVの生成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// SRVの設定
	srvDesc.Format = textureData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels);

	// SRVの作成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}

///-------------------------------------------/// 
/// SRVインデックスの開始番号
///-------------------------------------------///
uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath) {

	// 読み込み済みのテクスチャを検索
	auto it = std::find_if(
		textureDatas_.begin(), textureDatas_.end(), [&](TextureData& data) {
			return data.filePath == filePath;
		});

	if (it != textureDatas_.end()) {
		// 読み込み済みならインデックスを返す
		//return static_cast<uint32_t>(std::distance(textureDatas_.begin(), it));
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas_.begin(), it));
		return textureIndex;
	}

	assert(0);
	return 0;
}

///-------------------------------------------/// 
/// GPUハンドルの取得
///-------------------------------------------///
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVHandleGPU(uint32_t textureIndex) {
	// 範囲外指定違反チェック
	assert(textureIndex < textureDatas_.size());

	TextureData& textureData = textureDatas_[textureIndex];
	return textureData.srvHandleGPU;
}

///-------------------------------------------/// 
/// ミップマップの作成
///-------------------------------------------///
DirectX::ScratchImage TextureManager::Load(const std::string& filePath) {
	
	// テクスチャファイルを読み込んでプログラムで扱えるよにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	// リリースでもエラーが出るようにする
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to load texture from file: " + filePath);
	}

	// ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to generate mipmaps for texture: " + filePath);
	}

	// ミップマップのデータを返す
	return mipImages;
}

///-------------------------------------------/// 
/// TextureResourceの作成
///-------------------------------------------///
ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {
	/// ***************************
	/// metadataを基にResourceの設定
	/// ***************************
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width); // Textureの幅
	resourceDesc.Height = UINT(metadata.height); // Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels); // mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行 or 配列Textureの配列数
	resourceDesc.Format = metadata.format; // TextureのFormat
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // Textureの次元数。普段使っているのは２次元

	/// ***************************
	/// 利用するHeapの設定
	/// ***************************
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM; // 細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置

	/// ***************************
	/// Resourceの生成
	/// ***************************
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = dxCommon_->GetDevice()->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ, // 初回のResourceState。Textureは基本読むだけ
		nullptr, // Clear最適値。
		IID_PPV_ARGS(&resource)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
	return resource;
}

///-------------------------------------------/// 
/// データを転送する
///-------------------------------------------///
void TextureManager::UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages) {

	// Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	// 全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {

		// MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);

		// Textureに転送
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			UINT(img->rowPitch),  // １ラインサイズ
			UINT(img->slicePitch) // １枚サイズ
		);
		assert(SUCCEEDED(hr));
	}
}