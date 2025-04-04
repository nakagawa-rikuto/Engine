#include "TextureManager.h"
// Engine
#include "Engine/Core/DXCommon.h"
#include "Engine/Core/StringUtility.h"
#include "Engine/System/Managers/SRVManager.h"
// c++
#include <cassert>

// StringUtility
using namespace StringUtility;

// ImGuiで0番を使用するため、１番から使用
uint32_t TextureManager::kSRVIndexTop_ = 1;

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
TextureManager::~TextureManager() {
	textureDatas_.clear();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// メタデータの取得
const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& Key) {
	TextureData& textureData = textureDatas_[Key];
	return textureData.metadata;
}
// GPUハンドルの取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVHandleGPU(const std::string& Key) {
	TextureData& textureData = textureDatas_[Key];
	return textureData.srvHandleGPU;
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// テクスチャの設定
void TextureManager::SetGraphicsRootDescriptorTable(
	ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex, std::string Key) {
	commandList->SetGraphicsRootDescriptorTable(rootParameterIndex, textureDatas_[Key].srvHandleGPU);
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void TextureManager::Initialize(DXCommon* dxCommon, SRVManager* srvManager) {

	assert(dxCommon);
	assert(srvManager);

	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

	// SRVの数と同数
	textureDatas_.reserve(SRVManager::kMaxSRVCount_);
}

///-------------------------------------------/// 
/// テクスチャファイルの読み込み	
///-------------------------------------------///
void TextureManager::LoadTexture(const std::string& key, const std::string& filePath) {

	// 読み込み済みのテクスチャを検索
	if (textureDatas_.contains(key)) {
		assert(srvManager_->AssertAllocate());
		return;
	}

	// テクスチャの上限チェック
	assert(srvManager_->Allocate());

	// テクスチャデータを追加して書き込む
	TextureData& textureData = textureDatas_[key];
	// テクスチャデータの読み込み
	textureData.filePath = filePath;
	DirectX::ScratchImage mipImages = Load(key, filePath); // ミップマップの作成
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = CreateTextureResource(textureData.metadata);
	// テクスチャを転送
	UploadTextureData(textureData.resource, mipImages);

	// SRVを作成するDescriptorHeapの場所設定
	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	// SRVの生成
	srvManager_->CreateSRVForTexture2D(
		textureData.srvIndex, textureData.resource.Get(), textureData.metadata.format, UINT(textureData.metadata.mipLevels));
}


///-------------------------------------------/// 
/// ミップマップの作成
///-------------------------------------------///
DirectX::ScratchImage TextureManager::Load(const std::string& key, const std::string& filePath) {
	
	// テクスチャファイルを読み込んでプログラムで扱えるよにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	// リリースでもエラーが出るようにする
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to load texture with key: " + key + ", from file: " + filePath);
	}

	// ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to generate mipmaps for texture with key: " + key + ", from file: " + filePath);
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
	ComPtr<ID3D12Resource> resource = nullptr;
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
void TextureManager::UploadTextureData(ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages) {

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