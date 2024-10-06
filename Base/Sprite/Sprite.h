#pragma once
#include "CBufferData.h"

/// ===前方宣言=== ///
class SpriteCommon;

/// <summary>
/// スプライト
/// </summary>
class Sprite {
public:/// ===Getter=== ///

	/// <summary>
	/// 座標
	/// </summary>
	/// <returns></returns>
	const Vector2& GetPosition() const;

	/// <summary>
	/// 回転
	/// </summary>
	/// <returns></returns>
	const float& GetRotation() const;

	/// <summary>
	/// サイズ
	/// </summary>
	/// <returns></returns>
	const Vector2& GetSize() const;

	/// <summary>
	/// 色
	/// </summary>
	/// <returns></returns>
	const Vector4& GetColor() const;

public:/// ===Setter=== ///

	/// <summary>
	/// 座標
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position);

	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="position"></param>
	void SetRotation(const float& rotation);

	/// <summary>
	/// サイズ
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size);

	/// <summary>
	/// 色
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color);

public: /// ===基本的な関数=== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SpriteCommon* spriteCommon);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	// SpriteCommon
	SpriteCommon* spriteCommon_ = nullptr;

	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData_ = nullptr;
	uint32_t* indexData_ = nullptr;
	MaterialData* materialData_ = nullptr;
	TransformationMatrix* wvpMatrixData_ = nullptr;
	TransformInfo transform_; 	// Transform(scale, rotate, transform)

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuffer_;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpBuffer_;

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// 座標
	Vector2 position_ = { 0.0f, 0.0f };

	// 回転
	float rotation_ = 0.0f;

	// サイズ
	Vector2 size_ = { 640.0f, 360.0f };

	// 色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
private:

	/// <summary>
	/// Resourceの作成関数
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateResource(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// VertexResourceへの書き込み
	/// </summary>
	void VertexDataWrite();

	/// <summary>
	/// IndexResourceへの書き込み
	/// </summary>
	void IndexDataWrite();

	/// <summary>
	/// Transform情報の書き込み
	/// </summary>
	/// <param name="transformInfo"></param>
	void TransformDataWrite();
};

