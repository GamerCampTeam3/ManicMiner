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
	HazardTypes::EHazardId	    m_eHazardIdentifier;
	cocos2d::Vec2				m_cAnchorPoint;
	CGCFactoryCreationParams&	m_rFactoryCreationParams;

public:

	CGCObjHazard(const cocos2d::Vec2& rcAnchorPoint, const HazardTypes::EHazardId eHazardId, CGCFactoryCreationParams& rcParamsInput);

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
		return m_eHazardIdentifier;
	}
};
#endif // #ifndef _GCOBJHAZARD_H_