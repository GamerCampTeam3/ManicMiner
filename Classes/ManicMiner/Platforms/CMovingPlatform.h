////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CMOVINGPLATFORM_H_
#define _CMOVINGPLATFORM_H_

#include "CPlatform.h"

// Forward declare direction enum
enum class EPlayerDirection;

class CMovingPlatform : public CPlatform
{
public:
	
	CMovingPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, const EPlayerDirection eDirectionLock );

	void VOnResourceAcquire() override;
	
	void VOnReset() override;


	// DirectionLock Getter
	const EPlayerDirection GetDirectionLock();
private:
	cocos2d::Animation* m_pcMovingAnim;
	const EPlayerDirection m_eDirectionLock;
};
#endif