#include "RootSignature.h"
// Engine
#include "Engine/Core/Logger.h"
#include "Engine/Core/DXCommon.h"
// c++
#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include <format>

/// <summary>
/// ルートシグネイチャの生成
/// </summary>
void RootSignature::Create(DXCommon* dxCommon, PipelineType Type) {
	HRESULT hr;

	
	if (Type == PipelineType::ForGround2D || Type == PipelineType::BackGround2D) { /// ===Obj2D=== ///
		/// ===RootSignatureの生成=== ///
		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureObj2D{};
		descriptionRootSignatureObj2D.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		/// ===DescriptroRangeの生成=== ///
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0; // 0から始める
		descriptorRange[0].NumDescriptors = 1; // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

		/// ===RootParameterの生成=== ///
		D3D12_ROOT_PARAMETER rootParameters[3]{};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号の指定

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号の指定

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		descriptionRootSignatureObj2D.pParameters = rootParameters; // ルートパラメータ配列へのポインタ
		descriptionRootSignatureObj2D.NumParameters = _countof(rootParameters); // 配列の長さ

		/// ===Samplerの設定=== ///
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1]{};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderRegister = 0;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		descriptionRootSignatureObj2D.pStaticSamplers = staticSamplers;
		descriptionRootSignatureObj2D.NumStaticSamplers = _countof(staticSamplers);

		// シリアライズしてバイナリにする
		hr = D3D12SerializeRootSignature(
			&descriptionRootSignatureObj2D, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		// バイナリを元に生成
		assert(signatureBlob_);
		hr = dxCommon->GetDevice()->CreateRootSignature(
			0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
		if (FAILED(hr)) {
			assert(SUCCEEDED(hr));
		}

	} else if (Type == PipelineType::Obj3D) { /// ===Obj3D=== ///
		/// ===RootSignatureの生成=== ///
		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureObj3D{};
		descriptionRootSignatureObj3D.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		/// ===DescriptroRangeの生成=== ///
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0; // 0から始める
		descriptorRange[0].NumDescriptors = 1; // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

		/// ===RootParameterの生成=== ///
		D3D12_ROOT_PARAMETER rootParameters[7] = {};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使用
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使用
		rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0を使用

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使用
		rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号0を使用

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使用
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[3].Descriptor.ShaderRegister = 1; // レジスタ番号1を使用

		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[4].Descriptor.ShaderRegister = 2; // レジスタ番号2を使用

		rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[5].Descriptor.ShaderRegister = 3; // レジスタ番号3を使用

		rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[6].Descriptor.ShaderRegister = 4; // レジスタ番号4を使用

		descriptionRootSignatureObj3D.pParameters = rootParameters; // ルートパラメータ配列へのポインタ
		descriptionRootSignatureObj3D.NumParameters = _countof(rootParameters); // 配列の高さ

		/// ===Samplerの設定=== ///
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderRegister = 0;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		descriptionRootSignatureObj3D.pStaticSamplers = staticSamplers;
		descriptionRootSignatureObj3D.NumStaticSamplers = _countof(staticSamplers);

		// シリアライズしてバイナリにする
		hr = D3D12SerializeRootSignature(&descriptionRootSignatureObj3D, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		// バイナリを元に生成
		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
		assert(SUCCEEDED(hr));

	} else if (Type == PipelineType::Particle) { /// ===Particle=== ///

		/// ===RootSignatureの生成=== ///
		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureParticle{};
		descriptionRootSignatureParticle.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		/// ===DescriptroRangeの生成=== ///
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0; // 0から始める
		descriptorRange[0].NumDescriptors = 1; // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

		/// ===RootParameterの生成=== ///
		D3D12_ROOT_PARAMETER rootParameters[3] = {};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0を使う

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
		rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange; // テーブルの中身の配列を指定
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // テーブルで利用可能

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		descriptionRootSignatureParticle.pParameters = rootParameters; // ルートパラメータ配列へのポインタ
		descriptionRootSignatureParticle.NumParameters = _countof(rootParameters); // 配列の高さ

		/// ===Samplerの設定=== ///
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderRegister = 0;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		descriptionRootSignatureParticle.pStaticSamplers = staticSamplers;
		descriptionRootSignatureParticle.NumStaticSamplers = _countof(staticSamplers);

		// シリアライズしてバイナリにする
		hr = D3D12SerializeRootSignature(&descriptionRootSignatureParticle, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		// バイナリを元に生成
		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
		assert(SUCCEEDED(hr));

	} else if (Type == PipelineType::Skinning3D) { /// ===Skinning3D=== ///
		/// ===RootSignatureの生成=== ///
		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureObj3D{};
		descriptionRootSignatureObj3D.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		/// ===DescriptroRangeの生成=== ///
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0; // 0から始める
		descriptorRange[0].NumDescriptors = 1; // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

		/// ===RootParameterの生成=== ///
		D3D12_ROOT_PARAMETER rootParameters[8] = {};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使用
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使用
		rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0を使用

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使用
		rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号0を使用

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使用
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange; // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[3].Descriptor.ShaderRegister = 1; // レジスタ番号1を使用

		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[4].Descriptor.ShaderRegister = 2; // レジスタ番号2を使用

		rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[5].Descriptor.ShaderRegister = 3; // レジスタ番号3を使用

		rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[6].Descriptor.ShaderRegister = 4; // レジスタ番号4を使用

		rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
		rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; 
		rootParameters[7].DescriptorTable.pDescriptorRanges = descriptorRange; // テーブルの中身の配列を指定
		rootParameters[7].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // テーブルで利用可能

		descriptionRootSignatureObj3D.pParameters = rootParameters; // ルートパラメータ配列へのポインタ
		descriptionRootSignatureObj3D.NumParameters = _countof(rootParameters); // 配列の高さ

		/// ===Samplerの設定=== ///
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderRegister = 0;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		descriptionRootSignatureObj3D.pStaticSamplers = staticSamplers;
		descriptionRootSignatureObj3D.NumStaticSamplers = _countof(staticSamplers);

		// シリアライズしてバイナリにする
		hr = D3D12SerializeRootSignature(&descriptionRootSignatureObj3D, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		// バイナリを元に生成
		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
		assert(SUCCEEDED(hr));

	} else {
		return;
	}
}

/// <summary>
///  ルートシグネイチャの取得
/// </summary>
/// <returns></returns>
ID3D12RootSignature* RootSignature::GetRootSignature() const { return rootSignature_.Get(); }