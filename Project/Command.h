#pragma once
#include "Object.h"

///=====================================================/// 
/// 抽象コマンド
///=====================================================///
class Command {
public : 
	virtual ~Command() = default;
	virtual void Execute(Object& object) = 0;
};

///=====================================================/// 
/// 左に移動するコマンド
///=====================================================///
class LeftCommand : public Command {
public: 
	void Execute(Object& object) override;
};

///=====================================================/// 
/// 右に移動するコマンド
///=====================================================///
class RightCommand : public Command {
public: 
	void Execute(Object& objcet) override;
};



