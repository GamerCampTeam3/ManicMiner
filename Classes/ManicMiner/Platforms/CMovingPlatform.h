////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CMOVINGPLATFORM_H_
#define _CMOVINGPLATFORM_H_

#include "CPlatform.h"

class CMovingPlatform : public CPlatform
{
public:
	
	CMovingPlatform(CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition );

	void VOnResourceAcquire() override;
	
	void VOnReset() override;

private:

	cocos2d::Animation* m_pcMovingAnim;
	
};

#endif
