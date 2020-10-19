////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJHAZARD_H_
#define _GCOBJHAZARD_H_

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "ManicMiner/Enums/EHazardTypes.h"

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

class CGCObjHazard
: public CGCObjSpritePhysics
{
public:
	
private:
	HazardTypes::EHazardId	    eHazardIdentifier;
	cocos2d::Vec2				m_cAnchorPoint;
	CGCFactoryCreationParams&	rFactoryCreationParams;

public:

	CGCObjHazard(const cocos2d::Vec2& AnchorPoint, const HazardTypes::EHazardId HazardIdentifierInput, CGCFactoryCreationParams& ParamsInput);

	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjHazard()
	{}

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;
	virtual void VOnResurrected		( void ) override;
	virtual void VOnUpdate			(float fTimeStep) override;

	
	inline  HazardTypes::EHazardId GetHazardIdentifier()
	{
		return eHazardIdentifier;
	}
};
#endif // #ifndef _GCOBJHAZARD_H_