#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <string>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <wrl.h>

#include "CBufferData.h"
#include "DXCommon.h"
#include "PipelineStateObject.h"

#include "Vector2.h"
#include "Vector4.h"
#include "Matrix3x3.h"


/// <summary>
/// スプライトデータ
/// </summary>
class Sprite {
public:/* 関数 */

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static Sprite* GetInstance();

	/* /////////////////////////////////////////////////////////////////////
								描画（前・後）処理
	*/ /////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/* /////////////////////////////////////////////////////////////////////
							        Bufferの生成
	*/ /////////////////////////////////////////////////////////////////////
	/// <summary>
	/// VertexBufferの生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInBytes"></param>
	void CreateVertexBuffer(ID3D12Device* device, size_t sizeInBytes);

	/// <summary>
	/// IndexBufferの生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInBytes"></param>
	void CreateIndexBuffer(ID3D12Device* device, size_t sizeInBytes);

	/* /////////////////////////////////////////////////////////////////////
									データの書き込み
	*/ /////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 三角形のデータの書き込み
	/// </summary>
	/// <param name="TriangleLeftBottomPositionData"></param>
	/// <param name="TriangleTopPositionData"></param>
	/// <param name="TriangleRightBottomPositionData"></param>
	void WriteTriangle(
		Vector2* leftBottom,
		Vector2* Top,
		Vector2* rightBottom);

	/// <summary>
	/// 四角形のデータ書き込み
	/// </summary>
	/// <param name="leftTop"></param>
	/// <param name="RightTop"></param>
	/// <param name="leftBottom"></param>
	/// <param name="RightBottom"></param>
	void WriteSquare(
		Vector2* leftTop,
		Vector2* rightTop,
		Vector2* leftBottom,
		Vector2* rightBottom
	);

	/* /////////////////////////////////////////////////////////////////////
							       Getter
	*/ /////////////////////////////////////////////////////////////////////
	/// <summary>
	/// VertexBufferの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetVertexBuffer();

	/// <summary>
	/// IndexBufferの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetIndexBuffer();

	/// <summary>
	/// VertexBufferViewの取得
	/// </summary>
	/// <returns></returns>
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();

	/// <summary>
	/// IndexBufferViewの取得
	/// </summary>
	/// <returns></returns>
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();

private:
	// DirecXCommon
	DXCommon* dxCommon_;
	
	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_; 
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_; 

	// バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
};

