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
//  The following features are provided:
//  - Movement between two anchor points (not restricted to horizontal/vertical).
//  - Parameter driven speed.
//  - Start position within the movement window.
//  - Initial facing direction.
//  - Initial movement direction.
//  - Ability to extend the destination anchor point (note correctly resets with the level).
//  - 

//
//////////////////////////////////////////////////////////////////////////

class CGCObjEnemy
: public CGCObjSpritePhysics
{
private:
	
	float						m_fSpeed;
	float						m_fInitialDistanceFromAnchor;    
	float						m_fMovementWindowLength;
	float						m_fPreviousXPos;
	float						m_fMoveDelta;
	float						m_kfOne = 1.0f;
	float						m_kfZero = 0.0f;
	bool						m_bMovingAwayFromAnchorPoint;
	bool						m_bSpriteIsFlippable;
	bool						m_bHasBeenCollided;
	bool						m_bTemporaryAnchorPositionActive;
	bool						m_bInitialiseToOne;
	bool						m_bEnemyJustReceivedANewDestination;
	std::string                 m_pszPlist;
	std::string                 m_psName;
	cocos2d::Vec2				m_cAnchorPoint;
	cocos2d::Vec2				m_cDest;
	cocos2d::Vec2				m_cCurrentPos;
	cocos2d::Vec2				m_cNewDestination;
	cocos2d::Vec2				m_cTemporaryAnchorPosition;
	cocos2d::Vec2				m_cOriginalDestination;

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
	virtual void VOnResourceAcquire		( void ) override;
	virtual void VOnResurrected			( void ) override;
	virtual void VOnUpdate				(float fTimeStep) override;
	virtual void VOnResourceRelease		() override;
	virtual void VOnReset				() override;
	virtual void VHandleFactoryParams	(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

	void ModifyEnemyDestinationPoint(cocos2d::Vec2& rcNewDestination);

};
#endif // #ifndef _GCOBJENEMY_H_