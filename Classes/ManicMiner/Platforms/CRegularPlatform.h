////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CREGULARPLATFORM_H_
#define _CREGULARPLATFORM_H_

#include "CPlatform.h"

class CRegularPlatform : public CPlatform
{
public:

	CRegularPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, EPlatformType PlatformType);

	void VOnResourceAcquire() override;

	void VOnReset() override;

};

#endif
