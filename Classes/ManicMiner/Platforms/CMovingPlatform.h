////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CMOVINGPLATFORM_H_
#define _CMOVINGPLATFORM_H_

#include "CPlatform.h"

class CMovingPlatform : public CPlatform
{
public:
	
	CMovingPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, EPlatformType PlatformType);

	void VOnResourceAcquire() override;
	
	void VOnReset() override;
	
};

#endif