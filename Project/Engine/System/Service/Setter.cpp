#include "Setter.h"
// c++
#include <cassert>
// ServiceLocator
#include "ServiceLocator.h"

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// OffScreenTypeの設定
void Setter::SetOffScreenType(OffScreenType type) {
	return ServiceLocator::GetOffScreenRenderer()->SetType(type);
}
