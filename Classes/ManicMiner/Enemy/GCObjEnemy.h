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
//
//////////////////////////////////////////////////////////////////////////

class CGCObjEnemy
: public CGCObjSpritePhysics
{
private:
	
	float						m_fSpeed;								// enemy speed read (from OGMO file).
	float						m_fInitialDistanceFromAnchor;			// initial distance along the enemy traverse line (from OGMO file).
	float						m_fPreviousXPos;						// used to determine the X axis direction of travel to determine if the sprite needs to be flipped so enemy pointing correct way.
	float						m_fMoveDelta;							// the LERP input which describes the enemies postion between the Anchor and Destination points, range is normally between 0 and 1,
																		//	 exceeding this range is used to trigger either end of the traverse window.
	float						m_kfOne = 1.0f;							// Used to avoid magic numbers in the code.
	float						m_kfZero = 0.0f;						// Used to avoid magic numbers in the code.
	bool						m_bMovingAwayFromAnchorPoint;			// if enemy to be initially moving towards or away from the anchor point (from OGMO file).
	bool						m_bSpriteIsFlippable;                   // if the enemy spirte should be flipped (x axis only) when it reaches either end of the traverse window. 
	bool						m_bTemporaryAnchorPositionActive;		// indicates that on the current traverse, the movement window is not its 'normal' size.
	bool						m_bInitialiseToOne;						// indicate s that m_fMoveDelta is initialised/reset to 1.  (Required when moving towards anchor from mid point).
	bool						m_bEnemyJustReceivedANewDestination;	// indicates that a game mechanic has triggered this cycle to priovide enemy with a new destination value.
	std::string                 m_psName;								// Animation name (from OGMO file).
	cocos2d::Vec2				m_cAnchorPoint;							// starting point of the enemies traverse window.
	cocos2d::Vec2				m_cDest;								// destination point of the enemies traverse window.
	cocos2d::Vec2				m_cCurrentPos;							// current position along the enemies traverse window.
	cocos2d::Vec2				m_cNewDestination;						// new destination position (see m_bEnemyJustReceivedANewDestination above).
	cocos2d::Vec2				m_cTemporaryAnchorPosition;				// holds the calculated arbitrary point position when m_fInitialDistanceFromAnchor is > 0.
	cocos2d::Vec2				m_cOriginalDestination;					// stores the original destination position of the enemy read from OGMO for cases when the destination is modified mid game.

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