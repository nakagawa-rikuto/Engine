#pragma once
/// ===include=== ///
// c++
#include <cstdint>
#include <array>

///-------------------------------------------/// 
/// PipelineType
///-------------------------------------------///
enum PipelineType {
	// 随時追加していく
	// 2D
	Obj2D,
	// 3D
	Obj3D,
	// Particle
	Particle,
	// 総数(これは最後にしなければいけない)
	CountOfPipelineType,
};

///-------------------------------------------/// 
/// BlendMode
///-------------------------------------------///
enum BlendMode {
	//!< ブレンドなし
	kBlendModeNone,

	//!< 通常ブレンド。
	KBlendModeNormal,

	//!< 加算
	kBlendModeAdd,

	//!< 減算
	kBlendModeSubtract,

	//!< 乗算
	kBlendModeMultily,

	//!< スクリーン
	kBlendModeScreen,

	// 総数(これは最後にしなければいけない)
	kCountOfBlendMode,
};


// 列挙型の全値を取得する関数
constexpr std::array<PipelineType, static_cast<size_t>(PipelineType::CountOfPipelineType)> AllPipelineTypes() {
	return { PipelineType::Obj2D, PipelineType::Obj3D, PipelineType::Particle };
}
constexpr std::array<BlendMode, static_cast<size_t>(BlendMode::kCountOfBlendMode)> AllBlendModes() {
	return { BlendMode::kBlendModeNone, BlendMode::KBlendModeNormal, BlendMode::kBlendModeAdd,
			 BlendMode::kBlendModeSubtract, BlendMode::kBlendModeMultily, BlendMode::kBlendModeScreen };
}