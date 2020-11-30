////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJENEMY_H_
#define _GCOBJENEMY_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "ManicMiner/Enums/EEnemyTypes.h"

//////////////////////////////////////////////////////////////////////////
//  This class defines an invidual instance of an enemy character.
//  Its purpose is to provide the functionality an enemy would require with regards to
//  velocity, direction flipping, horizontal/vertical axis alignment, bouncing on 
//  platform collision etc.
//
//////////////////////////////////////////////////////////////////////////

class CGCObjEnemy
: public CGCObjSpritePhysics
{
private:

	cocos2d::Vec2				m_cAnchorPoint;
	float						m_fSpeed;
	float						m_fInitialDistanceFromAnchor;    
	float						m_fMovementWindowLength;
	bool						m_bMovingAwayFromAnchorPoint;
	bool						m_bInitialyMovingAwayFromAnchorPoint;
	bool                        m_bBounceCollisionDisabled;
	bool						m_bSpriteIsFlippable;
	bool						m_bHasBeenCollided;
	
	std::string                 m_pszPlist;

	cocos2d::Vec2				m_cDest;
	cocos2d::Vec2				m_cCurrentPos;
	cocos2d::Vec2				m_cNewDestination;
	std::string                 m_psName;
	float						m_fMoveDelta;
	bool						m_bTemporaryAnchorPositionActive;
	cocos2d::Vec2				m_cTemporaryAnchorPosition;
	bool						m_bInitialiseToOne;
	float						m_fPreviousXPos;
	bool						m_bEnemyJustReceivedANewDestination;

	std::unique_ptr< CGCFactoryCreationParams > m_pCustomCreationParams;

	bool CheckForBoundaryReached(const float fCurrentPosition, const float fAnchorPoint, const float fMovementWindowLength);
	bool CheckForDirectionFlip	();
	void SetFacingOrientation	();

protected:

	// This is proteced so sub classes like Eugene can access to change the animation mid game when triggered.
	cocos2d::Animation* pAnimation;
	std::string			m_pszAnimation;

public:

	CGCObjEnemy();

	CGCObjEnemy(GCTypeID idDerivedType);

	GCFACTORY_DECLARE_CREATABLECLASS(CGCObjEnemy);
	
	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjEnemy();
	
	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;
	virtual void VOnResurrected		( void ) override;
	virtual void VOnUpdate			(float fTimeStep) override;
	virtual void VOnResourceRelease	() override;
	virtual void VOnReset() override;

	void ModifyEnemyDestinationPoint(cocos2d::Vec2& rcNewDestination);
	virtual void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

};
#endif // #ifndef _GCOBJENEMY_H_