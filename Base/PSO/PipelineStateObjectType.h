#pragma once
/// ===include=== ///
// c++
#include <cstdint>
#include <array>

///-------------------------------------------/// 
/// PipelinType
///-------------------------------------------///
enum PipelinType {
	// 随時追加していく
	Obj2D,
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

	// 利用しない
	kCountOfBlendMode,
};