////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CBRICKPLATFORM_H_
#define _CBRICKPLATFORM_H_

#include "CPlatform.h"

class CBrickPlatform : public CPlatform
{
public:

	CBrickPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, EPlatformType PlatformType);

	void VOnResourceAcquire() override;

	void VOnReset() override;

};

#endif