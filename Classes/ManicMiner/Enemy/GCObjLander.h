////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJLANDER_H_
#define _GCOBJLANDER_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
	#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif

#include <string>
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "ManicMiner/Enums/EEnemyTypes.h"

class CGCObjLander
: public CGCObjSpritePhysics
{

public:

private:
	
	cocos2d::Vec2				m_cAnchorPoint;
	cocos2d::Vec2				m_cTotalVelocity;
	float						fSpeed;
	CGCFactoryCreationParams&	rFactoryCreationParams;
	
public:

	CGCObjLander(const cocos2d::Vec2& AnchorPoint, const float fSpeed, CGCFactoryCreationParams& ParamsInput);

	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjLander()
	{}

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;

	virtual void VOnResurrected		( void ) override;

	virtual void CGCObjLander::VOnUpdate(float fTimeStep) override;
	
};
#endif // #ifndef _GCOBJENEMY_H_