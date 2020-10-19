////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJENEMY_H_
#define _GCOBJENEMY_H_

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "ManicMiner/Enums/EEnemyTypes.h"


//////////////////////////////////////////////////////////////////////////
//  This class provides the lowest sub class of the enemy inheritance heirarchy.
//  Its purpose is to define the functionality an enemy would require with regards to
//  velocity, direction flipping, horizontal/vertical axis alignment, bouncing on 
//  platform collision etc.
// 
// Future improvements:
// This class could be sub classed into separate classes for Horizontal/Vertical enemy types.
// The advantage of this would be no need to be testing the enum input EMovementAxis
// at several points in the code, ie. the EMovementAxis input into the constructor would
// be removed as not required.
// There could also be a sub class for the collision bouncing functionality as this
// is only required for one enemy in one specific level.
// 
// I have decided to delay the above re-factoring until Module 2 and instead focus on 
// achieving proof of concept.
//
//////////////////////////////////////////////////////////////////////////

class CGCObjEnemy
: public CGCObjSpritePhysics
{
public:
	enum EMovementAxis { EMovementAxis_UpDown, EMovementAxis_LeftRight };

	void BounceEnemyDirection();

private:
	EMovementAxis				eMovementAxis;
	EnemyTypes::EEnemyId	    eEnemyIdentifier;
	cocos2d::Vec2				m_cAnchorPoint;
	cocos2d::Vec2				m_cTotalVelocity;
	float						fSpeed;
	float						fInitialDistanceFromAnchor;
	float						fMovementWindowLength;
	bool						bMovingAWayFromAnchorPoint;
	bool						bInitialyMovingAwayFromAnchorPoint;
	bool                        bBounceIsLatchedDisabled;
	bool						bSpriteIsFlippable;
	bool						m_bHasBeenCollided;
	const bool					k_bArtDefaultIsEnemyFacingRight;
	CGCFactoryCreationParams&	rFactoryCreationParams;

	bool CheckForBoundaryReached(const float fCurrentPosition, const float fAnchorPoint, const float fMovementWindowLength);

	bool CheckForDirectionFlip	();
	void SetFacingOrientation	();
		
public:

	CGCObjEnemy(const EMovementAxis EMovementAxisInput, const cocos2d::Vec2& AnchorPoint, const float fMovementRange, const float fInitialDistanceFromAnchor,
		bool bMovingAwayFromAnchorPoint, const float fSpeed, const bool bSpriteIsFlippable, const EnemyTypes::EEnemyId EnemyIdentifierInput,
		CGCFactoryCreationParams& ParamsInput);

	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjEnemy()
	{}

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;
	virtual void VOnResurrected		( void ) override;
	virtual void VOnUpdate			(float fTimeStep) override;


	

	// Bib Edit
	const bool GetHasCollided() { return m_bHasBeenCollided; };
	void SetHasCollided( bool collided ) { m_bHasBeenCollided = collided; };
	
	inline  EnemyTypes::EEnemyId GetEnemyIdentifier()
	{
		return eEnemyIdentifier;
	}
};
#endif // #ifndef _GCOBJENEMY_H_