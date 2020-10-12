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
						 EEnemyIdentifier EnemyIdentifierInput, CGCFactoryCreationParams& ParamsInput)

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
	, bFlipisCurrentLatchedDisabled(false)
{
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGCObject interface

//////////////////////////////////////////////////////////////////////////

// 
//////////////////////////////////////////////////////////////////////////


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
	

	
	GetCollisionManager().AddCollisionHandler([&](CGCObjItem& rcItem, CGCObjEnemy& rEnemy, const b2Contact& rcContact) -> void
		{
			bMovingAWayFromAnchorPoint = !bMovingAWayFromAnchorPoint;
		});

		

}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjEnemy::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}



bool CGCObjEnemy::checkForDirectionFlip()
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
	
	if (checkForDirectionFlip())
	{
        // if the Enemy's direction has just flipped then need to change the facing orientation
		// for left/right orientation Enemy's.
		if (EMovementAxis_LeftRight == eMovementAxis)
		{
			SetFacingOrientation();
		}
	}
	
}

void CGCObjEnemy::FlipEnemyDirection()
{
	if (!bFlipisCurrentLatchedDisabled)
	{

		if (EMovementAxis_LeftRight == eMovementAxis)
		{
			SetFlippedX(!bMovingAWayFromAnchorPoint);
		}
		bMovingAWayFromAnchorPoint = !bMovingAWayFromAnchorPoint;
	}
	bFlipisCurrentLatchedDisabled = true;
}

void CGCObjEnemy::ResetFlipLatch()
{
	bFlipisCurrentLatchedDisabled = false;
}




