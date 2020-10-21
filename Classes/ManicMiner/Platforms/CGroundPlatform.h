////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CGROUNDPLATFORM_H_
#define _CGROUNDPLATFORM_H_

#include "CPlatform.h"

class CGroundPlatform : public CPlatform
{
public:

	CGroundPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, EPlatformType PlatformType);

	void VOnResourceAcquire() override;

	void VOnReset() override;

};

#endif
