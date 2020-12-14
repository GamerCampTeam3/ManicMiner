////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer December 2020 - Module 2
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJHAZARD_H_
#define _GCOBJHAZARD_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

//////////////////////////////////////////////////////////////////////////
//
//  This class defines an invidual instance of a Hazard.
//  The following features are provided:
//  - initialise via OGMO parameters (VOnResourceAcquire).
//  - Once colided, disable collision detection to avoid multiple player deaths.
//	- 
// Known limitations:
// - None
//////////////////////////////////////////////////////////////////////////

class CGCObjHazard
: public CGCObjSpritePhysics
{
private:

	cocos2d::Vec2				m_cAnchorPoint;		// starting point of the enemies traverse window.
	std::string					m_pszAnimation;		// Pointer to the animation control structure.
	cocos2d::Animation*			pAnimation;			// Animation name (from OGMO file).
	bool						m_bCanCollide;		// Latching mechanism to avoid multiple triggers occuring when the player collides with a Hazard.
													// ie. set to false by the framework immediately following a collision, then set to true by VOnResurrected here when level resets.

	std::unique_ptr< CGCFactoryCreationParams > m_pCustomCreationParams;

public:

	CGCObjHazard();

	GCFACTORY_DECLARE_CREATABLECLASS(CGCObjHazard);

	virtual ~CGCObjHazard()
	{}

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the gamer camp interface
	virtual void VOnResourceAcquire		( void ) override;
	virtual void VOnResurrected			( void ) override;
	virtual void VOnResourceRelease		(void) override;
	virtual void VHandleFactoryParams	(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

	// Accessor functions for m_bCanCollide.
	bool GetCanCollide () { return m_bCanCollide; }
	void SetCanCollide ( bool bCanCollide ) { m_bCanCollide = bCanCollide; }

};
#endif // #ifndef _GCOBJHAZARD_H_