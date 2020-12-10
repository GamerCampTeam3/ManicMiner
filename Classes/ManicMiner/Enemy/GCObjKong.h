////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer December 2020 - Module 2
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJKONG_H_
#define _GCOBJKONG_H_

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

//////////////////////////////////////////////////////////////////////////
// This class defines an the Kong boss character.  
// Note Kong is not OGMO driven - he is instantiated via the Module 1 method.
//
// The following functionality is provided:
// -  Trigger function to activate Kong to flip his Y axis sprite and 
//	  fall off the screen when required.
//
// Known limitations:
// - None
//
// Improvements:
// - If the game introduces an event driven system, then the VUpdate logic
//   would be an ideal candidate to be event driven rather than waisting
//   processing in VUpdate every cycle.
//
//////////////////////////////////////////////////////////////////////////

class CGCObjKong
: public CGCObjSpritePhysics
{
private:

	// Kong's gameplay state.
	enum class EKongState {
		EWaitingToFall,
		EFalling,
	};

	EKongState					m_cKongState;						// Identifies Kongs current gameplay state - waiting or falling.
	cocos2d::Vec2				m_cAnchorPoint;						// Kongs on screen position before he falls.
	float						m_kfKongFallDuration = 4.0f;		// Fall duration time to ensure he disapears off bottom of the screen.
	float						m_kfKongFallVelocity = -10.0f;		// Fall velocity Y axis.
	float						m_currentTime;						// Used to store Kong's falling time window.
	CGCFactoryCreationParams&	m_rFactoryCreationParams;			// Factory creation params passed in when Kong is instantiated.
	std::string					m_pszAnimation;						// Kong's animation name.
	cocos2d::Animation*			pAnimation;							// Point to Kongs animation structure - created if the animation name length is > 0.
	bool						m_bKongIsFalling;					// Set by trigger function to activate Kongs falling logic.

public:
	

	CGCObjKong(const cocos2d::Vec2& rcAnchorPoint, const cocos2d::Vec2& rcDestinationPoint, const float fSpeed, CGCFactoryCreationParams& ParamsInput);

	virtual ~CGCObjKong();
	
	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;
	virtual void VOnResurrected		( void ) override;
	virtual void VOnUpdate			(float fTimeStep) override;
	virtual void VOnResourceRelease	() override;
	virtual void VOnReset			() override;

	//////////////////////////////////////////////////////////////////////////
	// specific public functions for this class
	void TriggerKongToFall();

};
#endif // #ifndef _GCOBJKONG_H_