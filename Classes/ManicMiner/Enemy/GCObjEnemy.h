////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer December 2020 - Module 2
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJENEMY_H_
#define _GCOBJENEMY_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

//////////////////////////////////////////////////////////////////////////
//  This class defines an invidual instance of an enemy character.
//
//  The following features are provided:
//  - Movement between two arbitrary anchor points (ie. not restricted to the Manic Miner required of horizontal/vertical onlu).
//  - Parameter driven speed.
//  - Arbitrary start position within the movement window.
//  - Initial movement direction.
//  - Ability to extend the destination anchor point (note correctly resets back with the level reset).
//
// Design:
// Enemy movement is provided using linear interpolation via the Vec2.LERP function between two Vec2 anchor
// points.  These anchor points can be at any arbitrary positions on the screen allowing for a full 360 range of
// movement angles.  
//
// Enemies are instantiated via the module 2 object factory method where data is read from a OGMO
// level file rather than instantiated by the Manic Miner game framework as in module 1.  The overriden operation 
// VHandleFactoryParams acts like a c++ constructor by populating member variables with construction data.
//
// Note! - Watch out for the easily made mistake of not realising that OGMO screen coordinates are top left and
// coocos2dx is bottom left.  You will see that the enemy destination X and Y parameters read from OGMO are 
// converted to cocos2dx in the overriden VHandleFactoryParams function.  
//
// Known limitations:
// - Sprite flipping to match movement direction is only provided for the X axis (Manic Miner
//   required only X axis sprite flipping).
// - In general due to Manic Miner requiring only horizontal or vertical movement, no real gameplay testing
//   has been done for diagonal movement, though when coordinates where incorrectly set during game development it did show
//   that diaganol movement was occuring between the anchor points.
// - The operation ModifyEnemyDestinationPoint can only be used to make the movement window larger.  Obviously making the 
//   movement window smaller is a totally different gameplay design situation as would need to take into account if the enemy
//   was outside the speicified smaller window when activated.
//
//////////////////////////////////////////////////////////////////////////

class CGCObjEnemy
: public CGCObjSpritePhysics
{
private:
	
	float						m_fSpeed;									// enemy speed read (from OGMO file).
	float						m_fInitialDistanceFromAnchor;				// configurable initial distance along the enemy traverse line (from OGMO file).
	float						m_fPreviousXPos;							// used to determine the X axis direction of travel to determine if the sprite needs to be flipped so enemy pointing correct way.
	float						m_fMoveDelta;								// the LERP input which describes the enemies postion between the Anchor and Destination points, range is normally between 0 and 1,
																			//	 exceeding this range is used to trigger either end of the traverse window logic.
	float						m_kfOne = 1.0f;								// Used to avoid magic numbers in the code.
	float						m_kfZero = 0.0f;							// Used to avoid magic numbers in the code.
	bool						m_bMovingAwayFromAnchorPoint;				// if enemy to be initially moving towards or away from the anchor point (from OGMO file).
	bool						m_bSpriteIsFlippable;						// if the enemy spirte should be flipped (x axis only) when it reaches either end of the traverse window. 
	bool						m_bTemporaryAnchorPositionActive;			// indicates that on the current traverse, the movement window is not its 'normal' size.
	bool						m_bInitialiseToOne;							// indicate s that m_fMoveDelta is initialised/reset to 1.  (Required when moving towards anchor from mid point).
	bool						m_bEnemyJustReceivedANewDestination;		// indicates that a game mechanic has triggered this cycle to priovide enemy with a new destination value.
	std::string                 m_psName;									// Object name when required (from OGMO file).
	cocos2d::Vec2				m_cAnchorPoint;								// starting point of the enemies traverse window.
	cocos2d::Vec2				m_cDest;									// destination point of the enemies traverse window.
	cocos2d::Vec2				m_cCurrentPos;								// current position along the enemies traverse window.
	cocos2d::Vec2				m_cNewDestination;							//new destination position (see m_bEnemyJustReceivedANewDestination above).
	cocos2d::Vec2				m_cTemporaryAnchorPosition;					// holds the calculated arbitrary point position when m_fInitialDistanceFromAnchor is > 0.
	cocos2d::Vec2				m_cOriginalDestination;						// stores the original destination position of the enemy read from OGMO for cases when the destination is modified mid game.
	std::unique_ptr< CGCFactoryCreationParams > m_pCustomCreationParams;	// Used by operation VHandleFactoryParams to override standard factory parameters with custom ones.


	//////////////////////////////////////////////////////////////////////////
	// specific private functions for this class
	void SetFacingOrientation();

protected:

	// These are protected so sub classes (eg.Eugene) can access to change the animation when triggered during gameplay.
	cocos2d::Animation*		pAnimation;				// Pointer to the animation control structure.
	std::string				m_pszAnimation;			// Animation name (from OGMO file).

public:

	CGCObjEnemy();

	GCFACTORY_DECLARE_CREATABLECLASS(CGCObjEnemy);
	
	virtual ~CGCObjEnemy();
	
	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the gamer camp interface
	virtual void VOnResourceAcquire		( void ) override;
	virtual void VOnResurrected			( void ) override;
	virtual void VOnUpdate				(float fTimeStep) override;
	virtual void VOnResourceRelease		() override;
	virtual void VOnReset				() override;
	virtual void VHandleFactoryParams	(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;


	//////////////////////////////////////////////////////////////////////////
	// specific public functions for this class
	void ModifyEnemyDestinationPoint(cocos2d::Vec2& rcNewDestination);

};
#endif // #ifndef _GCOBJENEMY_H_