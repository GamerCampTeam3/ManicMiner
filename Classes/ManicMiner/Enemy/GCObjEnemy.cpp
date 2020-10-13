////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>

#include "GamerCamp/GCObject/GCObjectManager.h"

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

#include "GCObjEnemy.h"

// REQUIRED FOR COLLISION HANDLER TO SEE
#include "GamerCamp/GameSpecific/Items/GCObjItem.h" 

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type

CGCObjEnemy::CGCObjEnemy(EMovementAxis EMovementAxisInput, cocos2d::Vec2 AnchorPoint, float MovementRange, float InitialDistanceFromAnchor, bool MovingAwayFromAnchorPoint, float Speed,
						 bool SpriteIsFlippable, EnemyTypes::EEnemyId EnemyIdentifierInput, CGCFactoryCreationParams& ParamsInput)

	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjEnemy))
	, eMovementAxis(EMovementAxisInput)
	, eEnemyIdentifier(EnemyIdentifierInput)
	, cAnchorPoint(AnchorPoint)
	, fMovementWindowLength(MovementRange)
	, fSpeed(Speed)
	, bMovingAWayFromAnchorPoint(MovingAwayFromAnchorPoint)
	, k_bArtDefaultIsEnemyFacingRight(false)
	, rFactoryCreationParams(ParamsInput)
	, fInitialDistanceFromAnchor (InitialDistanceFromAnchor)
	, bBounceIsLatchedDisabled(false)
	, bSpriteIsFlippable(SpriteIsFlippable)
{
}

//IN_CPP_CREATION_PARAMS_DECLARE( CGCObjEnemy, "TexturePacker/Sprites/KoopaTrooper/KoopaTrooper.plist", "koopa", b2_dynamicBody, true );
//virtual 
void CGCObjEnemy::VOnResourceAcquire( void )
{

	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjEnemy );    
	VHandleFactoryParams(rFactoryCreationParams, GetResetPosition());

	CGCObjSpritePhysics::VOnResourceAcquire();

	TotalVelocity = Vec2::ZERO;

	if (EMovementAxis_LeftRight == eMovementAxis)
	{

		TotalVelocity.x = fSpeed;

		Vec2 AnchorPointAndOffset = cAnchorPoint;
		AnchorPointAndOffset.x += fInitialDistanceFromAnchor;
		SetResetPosition(AnchorPointAndOffset);
	}
	else
	{
		TotalVelocity.y = fSpeed;

		Vec2 AnchorPointAndOffset = cAnchorPoint;
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

	if (EMovementAxis_LeftRight == eMovementAxis)
	{
		// Test right side boundary limit passed and flip to moving back to start if required.
		if (bMovingAWayFromAnchorPoint && CurrentPosition.x >= cAnchorPoint.x + fMovementWindowLength)
		{
			bMovingAWayFromAnchorPoint = false;
			return true;
		}
		// Test left side boundary limit passed and flip to moving away from start if required.
		if (!bMovingAWayFromAnchorPoint && CurrentPosition.x <= cAnchorPoint.x)
		{
			bMovingAWayFromAnchorPoint = true;
			return true;
		}
	}
	else
	{
		// Test upper boundary limit passed and flip to moving back to start if required.
		if (bMovingAWayFromAnchorPoint && CurrentPosition.y >= cAnchorPoint.y + fMovementWindowLength)
		{
			bMovingAWayFromAnchorPoint = false;
			return true;
		}
		// Test lower boundary limit passed and flip to moving away from start if required.
		if (!bMovingAWayFromAnchorPoint && CurrentPosition.y <= cAnchorPoint.y)
		{
			bMovingAWayFromAnchorPoint = true;
			return true;
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjEnemy::VOnUpdate(float fTimeStep)
{
	CGCObject::VOnUpdate(fTimeStep);

	if (bMovingAWayFromAnchorPoint)
	{
		SetVelocity(TotalVelocity);
	}
	else
	{
		SetVelocity(-TotalVelocity);
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
	if (k_bArtDefaultIsEnemyFacingRight)
	{
		SetFlippedX(!bMovingAWayFromAnchorPoint);
	}
	else
	{
		SetFlippedX(bMovingAWayFromAnchorPoint);
	}
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
	bBounceIsLatchedDisabled = true;
}
