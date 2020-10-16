////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>

#include "GamerCamp/GCObject/GCObjectManager.h"

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

#include "GCObjEnemy.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type


CGCObjEnemy::CGCObjEnemy(const EMovementAxis EMovementAxisInput, const cocos2d::Vec2& AnchorPoint, const float fMovementRange, const float fInitialDistanceFromAnchor,
	bool bMovingAwayFromAnchorPoint, const float fSpeed, const bool bSpriteIsFlippable, const EnemyTypes::EEnemyId EnemyIdentifierInput,
	CGCFactoryCreationParams& ParamsInput)

	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjEnemy))
	, eMovementAxis(EMovementAxisInput)
	, eEnemyIdentifier(EnemyIdentifierInput)
	, m_cAnchorPoint(AnchorPoint)
	, fMovementWindowLength(fMovementRange)
	, fSpeed(fSpeed)
	, bMovingAWayFromAnchorPoint(bMovingAwayFromAnchorPoint)
	, k_bArtDefaultIsEnemyFacingRight(false)
	, rFactoryCreationParams(ParamsInput)
	, fInitialDistanceFromAnchor (fInitialDistanceFromAnchor)
	, bBounceIsLatchedDisabled(false)
	, bSpriteIsFlippable(bSpriteIsFlippable)
{
}

//IN_CPP_CREATION_PARAMS_DECLARE( CGCObjEnemy, "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist", "koopa", b2_dynamicBody, true );
//virtual 
void CGCObjEnemy::VOnResourceAcquire( void )
{

	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjEnemy );    
	VHandleFactoryParams(rFactoryCreationParams, GetResetPosition());

	CGCObjSpritePhysics::VOnResourceAcquire();

	m_cTotalVelocity = Vec2::ZERO;

	if (EMovementAxis_LeftRight == eMovementAxis)
	{

		m_cTotalVelocity.x = fSpeed;

		Vec2 AnchorPointAndOffset = m_cAnchorPoint;
		AnchorPointAndOffset.x += fInitialDistanceFromAnchor;
		SetResetPosition(AnchorPointAndOffset);
	}
	else
	{
		m_cTotalVelocity.y = fSpeed;

		Vec2 AnchorPointAndOffset = m_cAnchorPoint;
		AnchorPointAndOffset.y += fInitialDistanceFromAnchor;
		SetResetPosition(AnchorPointAndOffset);
	}

	SetFacingOrientation();
}

//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjEnemy::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
// Function to check the boundaries of the enemy movement window to detect a direction flip.
// Returns true if a directional flip was perfomed.
//
bool CGCObjEnemy::CheckForDirectionFlip()
{
	Vec2 CurrentPosition = GetSpritePosition();

	bool bReturnResult = false;

	if (EMovementAxis_LeftRight == eMovementAxis)
	{
		// Test right side boundary limit passed and flip to moving back to start if required.
		if (bMovingAWayFromAnchorPoint && CurrentPosition.x >= m_cAnchorPoint.x + fMovementWindowLength)
		{
			bMovingAWayFromAnchorPoint = false;
			bReturnResult = true;
		}
		// Test left side boundary limit passed and flip to moving away from start if required.
		if (!bMovingAWayFromAnchorPoint && CurrentPosition.x <= m_cAnchorPoint.x)
		{
			bMovingAWayFromAnchorPoint = true;
			bReturnResult = true;
		}
	}
	else
	{
		// Test upper boundary limit passed and flip to moving back to start if required.
		if (bMovingAWayFromAnchorPoint && CurrentPosition.y >= m_cAnchorPoint.y + fMovementWindowLength)
		{
			bMovingAWayFromAnchorPoint = false;
			bReturnResult = true;
			
		}
		// Test lower boundary limit passed and flip to moving away from start if required.
		if (!bMovingAWayFromAnchorPoint && CurrentPosition.y <= m_cAnchorPoint.y)
		{
			bMovingAWayFromAnchorPoint = true;
			bReturnResult = true;
		}
	}
	return bReturnResult;
}


//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjEnemy::VOnUpdate(float fTimeStep)
{
	CGCObject::VOnUpdate(fTimeStep);

	if (bMovingAWayFromAnchorPoint)
	{
		SetVelocity(m_cTotalVelocity);
	}
	else
	{
		SetVelocity(-m_cTotalVelocity);
	}
	
	if (CheckForDirectionFlip())
	{

		bBounceIsLatchedDisabled = false;

        // if the Enemy's direction has just flipped then need to change the facing orientation
		// for left/right orientation Enemy's.
		if (EMovementAxis_LeftRight == eMovementAxis && bSpriteIsFlippable)
		{
			SetFacingOrientation();
		}
	}

}
//////////////////////////////////////////////////////////////////////////
// Function to set the facing orientation of the enemy sprite.
//
void CGCObjEnemy::SetFacingOrientation()
{
	SetFlippedX((k_bArtDefaultIsEnemyFacingRight && !bMovingAWayFromAnchorPoint) || (!k_bArtDefaultIsEnemyFacingRight && bMovingAWayFromAnchorPoint));
}
//////////////////////////////////////////////////////////////////////////
// Function to flip the enemies current direction of travel when it has collided with an object.
// 
void CGCObjEnemy::BounceEnemyDirection()
{
	// If the enemy has just collided with an ojbect then this latching flag is set true to allow only
	// one flip of the sprite during collisions lifttime.   
	// The flag is reset when the enemy reaches the boundary at the other
	// side of its movement window.   This is a crude method but sufficient for the one edge case when
	// an enemy collides with an object which restricts the enemys moviement.  During gameplay the 
	// ojbect is removed therefore allowing greater movement of the enemy.

	if (!bBounceIsLatchedDisabled)
	{
		if (EMovementAxis_LeftRight == eMovementAxis && bSpriteIsFlippable)
		{
			SetFlippedX(!bMovingAWayFromAnchorPoint);
		}
		bMovingAWayFromAnchorPoint = !bMovingAWayFromAnchorPoint;
	}
	// Latch the flag to stop the enemy constantly flipping during the collisions duration.
	bBounceIsLatchedDisabled = true;
}
