#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Core/ComPtr.h"

// PipelineType
#include "Game/Data/PipelineStateObjectType.h"

// directX
#include <d3d12.h>
#include <dxcapi.h>

// c++
#include <string>

#pragma comment(lib, "dxcompiler.lib")

/// ===前方宣言=== ///
class DXCommon;

///=====================================================/// 
/// Compiler
///=====================================================///
class Compiler {
public:

	Compiler() = default;
	~Compiler() = default;

	/// <summary>
	/// shaderをコンパイルする
	/// </summary>
	void Initialize(DXCommon* dxCommon, PipelineType Type);

	/// <summary>
	/// ObjectVertexShaderBlobの取得
	/// </summary>
	/// <returns></returns>
	IDxcBlob* GetObjVS();

	/// <summary>
	/// ObjectPixelShaderBlobの取得
	/// </summary>
	/// <returns></returns>
	IDxcBlob* GetObjPS();

private:

	//　ObjectShader
	ComPtr<IDxcBlob> objVSBlob_;
	ComPtr<IDxcBlob> objPSBlob_;

	/// <summary>
	/// CompileShader関数
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="profile"></param>
	/// <param name="dxcUtils"></param>
	/// <param name="dxcCompiler"></param>
	/// <param name="includeHandler"></param>
	/// <returns></returns>
	ComPtr<IDxcBlob> CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,

		// Compilerに使用するProfile
		const wchar_t* profile,

		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

};