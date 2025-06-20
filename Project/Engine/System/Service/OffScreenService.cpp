#include "OffScreenService.h"
// c++
#include <cassert>
// ServiceLocator
#include "ServiceLocator.h"

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// OffScreenTypeの設定
void OffScreenService::SetOffScreenType(OffScreenType type) {
	return ServiceLocator::GetOffScreenRenderer()->SetType(type);
}
