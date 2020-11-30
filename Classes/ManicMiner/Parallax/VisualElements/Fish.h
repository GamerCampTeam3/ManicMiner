////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CFISH_H_
#define _CFISH_H_

#ifndef _GCFACTORY_OBJSPRITE_H_
#include "GamerCamp/GCObject/GCObject.h"
#endif
#include "2d/CCSprite.h"


class CGCObjSprite;

class CFish
	: public CGCObject
{

public:
	
	CFish();
	~CFish();

	void VOnResourceAcquire() override;

	void VOnResourceRelease() override;
	
	void VOnReset() override;

	void VOnResurrected() override;

	void VOnUpdate(float fTimeStep) override;
	
private:

	// The Visual Element of the Fish that will be moving around
	CGCObjSprite* m_pcSprFish;
	

	// the current Location the fish is moving towards
	cocos2d::Vec2 m_v2CurrentMoveToLocation;
	// the next Location that the fish will start moving towards, once it has reached 2/3 of the distance from when it started moving to the current location
	cocos2d::Vec2 m_v2NextMoveToLocation;

	// get 2 random floats in range (x = (min = 200.f, max = 1720.f), y = (min = 100.f, max = 860.f)) 
	cocos2d::Vec2 FindRandomMoveToLocation();

	// Gets current location of the fish and subtracts it by the location that it is about to move to.
	void GetDistanceToCurrentMoveToLocation();

public:

	void Init();
};

#endif

