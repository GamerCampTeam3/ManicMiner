////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjEnemy.h"

USING_NS_CC;

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

//////////////////////////////////////////////////////////////////////////
// This function initialises an enemies direction of velocity, 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjEnemy::VOnResourceAcquire( void )
{

	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjEnemy );    
	VHandleFactoryParams(rFactoryCreationParams, GetResetPosition());

	CGCObjSpritePhysics::VOnResourceAcquire();

	m_cTotalVelocity = Vec2::ZERO;

	Vec2 AnchorPointAndOffset = m_cAnchorPoint;
	if (EMovementAxis_LeftRight == eMovementAxis)
	{
		m_cTotalVelocity.x = fSpeed;
		AnchorPointAndOffset.x += fInitialDistanceFromAnchor;
	}
	else
	{
		m_cTotalVelocity.y = fSpeed;
		AnchorPointAndOffset.y += fInitialDistanceFromAnchor;
	}

	SetResetPosition(AnchorPointAndOffset);

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
//virtual function
bool CGCObjEnemy::CheckForBoundaryReached(const float fCurrentPosition, const float fAnchorPoint, const float fMovementWindowLength)
{
	bool bReturnResult = false;

	// Test right or upper side boundary limit passed and flip to moving back to start if required.
	if (bMovingAWayFromAnchorPoint && (fCurrentPosition >= fAnchorPoint + fMovementWindowLength))
	{
		bMovingAWayFromAnchorPoint = false;
		bReturnResult = true;
	}
	// Test left or lower side boundary limit passed and flip to moving away from start if required.
	else if (!bMovingAWayFromAnchorPoint && (fCurrentPosition <= fAnchorPoint))
	{
		bMovingAWayFromAnchorPoint = true;
		bReturnResult = true;
	}
	return bReturnResult;
}


//////////////////////////////////////////////////////////////////////////
// Function to check the boundaries of the enemy movement window to detect a directional flip.
// Returns true if a directional flip was perfomed.
//
bool CGCObjEnemy::CheckForDirectionFlip()
{
	Vec2 CurrentPosition = GetSpritePosition();
	if (EMovementAxis_LeftRight == eMovementAxis)
	{
		return CheckForBoundaryReached(CurrentPosition.x, m_cAnchorPoint.x, fMovementWindowLength);
	}
	else
	{
		return CheckForBoundaryReached(CurrentPosition.y, m_cAnchorPoint.y, fMovementWindowLength);
	}
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
