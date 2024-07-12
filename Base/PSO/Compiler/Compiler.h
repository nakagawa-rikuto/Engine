#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

#include "DXCommon.h"

#pragma comment(lib, "dxcompiler.lib")

class Compiler {

public:

	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static Compiler* GetInstance();

	/// <summary>
	/// shaderをコンパイルする
	/// </summary>
	void ShaderCompiler(DXCommon* dxCommon);

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
	Microsoft::WRL::ComPtr<IDxcBlob> objVSBlob_;
	Microsoft::WRL::ComPtr<IDxcBlob> objPSBlob_;

	/// <summary>
	/// CompileShader関数
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="profile"></param>
	/// <param name="dxcUtils"></param>
	/// <param name="dxcCompiler"></param>
	/// <param name="includeHandler"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,

		// Compilerに使用するProfile
		const wchar_t* profile,

		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

};