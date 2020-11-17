////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJHAZARD_H_
#define _GCOBJHAZARD_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "ManicMiner/Enums/EHazardTypes.h"

//////////////////////////////////////////////////////////////////////////
//
//  This class defines an invidual instance of a Hazard.
//  Its purpose is to provide the functionality a Hazard would require with regards to
//  screen position and an identifier for the Hazard (EHazardID)
//
//////////////////////////////////////////////////////////////////////////

class CGCObjHazard
: public CGCObjSpritePhysics
{
public:
	
private:
	HazardTypes::EHazardId	    m_eHazardIdentifier;
	cocos2d::Vec2				m_cAnchorPoint;


	// For animations
	std::string					m_pszAnimation;
	std::string                 m_pszPlist;
	cocos2d::Animation*			pAnimation;




	//CGCFactoryCreationParams&	m_rFactoryCreationParams;


	std::unique_ptr< CGCFactoryCreationParams > m_pCustomCreationParams;


public:

	CGCObjHazard();

	// Module 1 constructor
	//CGCObjHazard(const cocos2d::Vec2& rcAnchorPoint, const HazardTypes::EHazardId eHazardId, CGCFactoryCreationParams& rcFactoryCreationParamsInput);
	   
	GCFACTORY_DECLARE_CREATABLECLASS(CGCObjHazard);

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
	virtual void VOnResourceRelease	(void) override;

	virtual void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

	inline  HazardTypes::EHazardId GetHazardIdentifier()
	{
		return m_eHazardIdentifier;
	}
};
#endif // #ifndef _GCOBJHAZARD_H_