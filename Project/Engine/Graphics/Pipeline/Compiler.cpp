#include "Compiler.h"

#include "Engine/Core/Logger.h"
#include "Engine/Core/DXCommon.h"

#include <cassert>

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Compiler::~Compiler() {
	objVSBlob_.Reset();
	objPSBlob_.Reset();
}

///-------------------------------------------/// 
/// shaderをコンパイルする
///-------------------------------------------///
void Compiler::Initialize(DXCommon* dxCommon, PipelineType Type) {

	if (Type == PipelineType::ForGround2D || Type == PipelineType::BackGround2D ) {

		// VS
		objVSBlob_ = CompileShader(L"Resource/Shaders/Obj2D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objVSBlob_ != nullptr);

		// PS
		objPSBlob_ = CompileShader(L"Resource/Shaders/Obj2D.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objPSBlob_ != nullptr);

	} else if (Type == PipelineType::Obj3D) {

		// VS
		objVSBlob_ = CompileShader(L"Resource/Shaders/Obj3D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objVSBlob_ != nullptr);

		// PS
		objPSBlob_ = CompileShader(L"Resource/Shaders/Obj3D.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objPSBlob_ != nullptr);

	} else if(Type == PipelineType::Particle){

		// VS
		objVSBlob_ = CompileShader(L"Resource/Shaders/Particle.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objVSBlob_ != nullptr);

		// PS
		objPSBlob_ = CompileShader(L"Resource/Shaders/Particle.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objPSBlob_ != nullptr);

	} else if (Type == PipelineType::Skinning3D) {

		// VS
		objVSBlob_ = CompileShader(L"Resource/Shaders/SkinningObj3D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objVSBlob_ != nullptr);

		// PS
		objPSBlob_ = CompileShader(L"Resource/Shaders/SkinningObj3D.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objPSBlob_ != nullptr);

	} else {

		// VS
		objVSBlob_ = CompileShader(L"Resource/Shaders/Obj.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objVSBlob_ != nullptr);

		// PS
		objPSBlob_ = CompileShader(L"Resource/Shaders/Obj.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(objPSBlob_ != nullptr);
	}
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// ObjectVertexShaderBlobの取得
IDxcBlob* Compiler::GetObjVS() { return objVSBlob_.Get(); }
// ObjectPixelShaderBlobの取得
IDxcBlob* Compiler::GetObjPS() { return objPSBlob_.Get(); }

///-------------------------------------------/// 
/// CompileShader関数
///-------------------------------------------///
ComPtr<IDxcBlob> Compiler::CompileShader(
	const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {

	HRESULT hr;

	/// ===1. hlslファイルを読み込む=== ///
	// これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));

	// hlslファイルを読む
	ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);

	// 読めなかったら止める
	assert(SUCCEEDED(hr));

	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF8の文字コードであることを通知

	/// ===2. Compileする === ///
#ifdef _DEBUG
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E", L"main",
		L"-T", profile,
		L"-Zi", L"-Qembed_debug",   // デバッグ情報あり
		L"-Od",                     // 最適化オフ
		L"-Zpr",
		L"-D", L"_DEBUG",           // _DEBUGマクロを定義
	};
#else
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E", L"main",
		L"-T", profile,
		// デバッグ情報はなし
		L"-O2",                     // 高速化のための最適化
		L"-Zpr",
		L"-D", L"NDEBUG",           // NDEBUGマクロを定義（必要なら）
	};
#endif

	// 実際にShaderをコンパイルする
	ComPtr<IDxcResult> shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,          // 読み込んだファイル
		arguments,                    // コンパイルオプション
		_countof(arguments),          // コンパイルオプションの数
		includeHandler,               // includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)   // コンパイル結果
	);

	// コンパイルエラーではなくDxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	/// ===3. 警告・エラーが出ていないかを確認する=== ///
	// 警告・エラーが出てたらログを出して止める
	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());

		// 警告・エラーダメゼッタイ
		assert(false);
	}

	/// ===4. Compile結果を受け取ってます=== ///
	// コンパイル結果から実行用のバイナリ部分を取得
	ComPtr<IDxcBlob> shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));

	// 成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));

	// 実行用のバイナリを返却
	return shaderBlob;
}
