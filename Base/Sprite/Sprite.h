#pragma once
#include "CBufferData.h"

/// ===前方宣言=== ///
class SpriteCommon;

/// <summary>
/// スプライト
/// </summary>
class Sprite {
public: /// ===基本的な関数=== ///

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static Sprite* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SpriteCommon* spriteCommon);

private:

	// SpriteCommon
	SpriteCommon* spriteCommon_ = nullptr;

	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	MaterialData* materialData_ = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuffer_;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_VERTEX_BUFFER_VIEW indexBufferView_{};

private:

	/// <summary>
	/// Resourceの作成関数
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateResource(ID3D12Device* device, size_t sizeInBytes);
};

