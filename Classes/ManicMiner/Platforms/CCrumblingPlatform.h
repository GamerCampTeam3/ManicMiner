////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CCRUMBLINGPLATFORM_H_
#define _CCRUMBLINGPLATFORM_H_

#include "CPlatform.h"

/*
enum class ECrumbleState
{
	0,
	1,
	2,
	3,
	Destroy
};*/

class CCrumblingPlatform : public CPlatform
{
public:
	
	CCrumblingPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition );

	void VOnResourceAcquire() override;

	void VOnUpdate(float fTimeStep) override;

	void VOnReset() override;

private:

	////////////////////////////////////////////////////////////////////////
	/// Crumbling Platforms

	ECrumbleState m_eCrumbleState;

	bool m_bInitiatedCrumbling;

	float m_fCurrentCrumblingTimer;
	float m_fReduceCrumblingTimerBy;

	class cocos2d::Director* m_pcDirector;

	cocos2d::Animation* m_pcCrumbleAnim;

	// Initiates Crumbling Timer and Animation
	void InitiateCrumbling(float fSecondsToStartCrumbling);

	void UpdateCrumblingPlatform(ECrumbleState eNewCrumbleState);

	/// Crumbling Platforms
	////////////////////////////////////////////////////////////////////////
	
};

#endif

