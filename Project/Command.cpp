#include "Command.h"

///-------------------------------------------/// 
/// 左
///-------------------------------------------///
void LeftCommand::Execute(Object& object) {
	object.Move(-0.8f);
}

///-------------------------------------------/// 
/// 右
///-------------------------------------------///
void RightCommand::Execute(Object& objcet) {
	objcet.Move(0.8f);
}
