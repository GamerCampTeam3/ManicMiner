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

//////////////////////////////////////////////////////////////////////////
//  This class defines an invidual instance of a Lander Enemy.
// NOTE NOT COMPLETE AT MODULE 1
//////////////////////////////////////////////////////////////////////////

class CGCObjLander
: public CGCObjSpritePhysics
{
private:
	cocos2d::Vec2				m_cAnchorPoint;
	cocos2d::Vec2				m_cTotalVelocity;
	float						m_fSpeed;
	CGCFactoryCreationParams&	m_rFactoryCreationParams;
	
public:

	CGCObjLander(const cocos2d::Vec2& rcAnchorPoint, const float fSpeed, CGCFactoryCreationParams& rcFactoryCreationParamsInput);

	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjLander()
	{}

	virtual void VOnResourceAcquire	( void ) override;
	virtual void VOnResurrected		( void ) override;
	virtual void VOnUpdate(float fTimeStep) override;

};
#endif // #ifndef _GCOBJENEMY_H_