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

CGCObjEnemy::CGCObjEnemy(EMovementAxis EMovementAxisInput, cocos2d::Vec2 AnchorPoint, float MovementWindowLength, float InitialDistanceFromAnchor, float SpeedInput, EEnemyIdentifier EnemyIdentifierInput, CGCFactoryCreationParams& ParamsInput,
						 cocos2d::Animation* pAnimationInput)

	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjEnemy))
	, eMovementAxis(EMovementAxisInput)
	, eEnemyIdentifier(EnemyIdentifierInput)
	, cAnchorPoint(AnchorPoint)
	, fMovementWindowLength(MovementWindowLength)
	, fSpeed(SpeedInput)
	, bMovingAWayFromStartPosition(true)
	, k_bArtDefaultIsEnemyFacingRight(false)
	, rFactoryCreationParams(ParamsInput)
	, pAnimation(pAnimationInput)
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

	SetResetPosition(Vec2(cAnchorPoint));
	SetFacingOrientation();

	TotalVelocity = Vec2::ZERO;

	if (EMovementAxis_LeftRight == eMovementAxis)
	{
		TotalVelocity.x = fSpeed;
	}
	else
	{
		TotalVelocity.y = fSpeed;
	}
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
		if (bMovingAWayFromStartPosition && CurrentPosition.x >= cAnchorPoint.x + fMovementWindowLength)
		{
			bMovingAWayFromStartPosition = false;
			return true;
		}

		// Test left side boundary limit passed and flip to moving away from start if required.
		if (!bMovingAWayFromStartPosition && CurrentPosition.x <= cAnchorPoint.x)
		{
			bMovingAWayFromStartPosition = true;
			return true;
		}
	}
	else
	{
		// Test upper boundary limit passed and flip to moving back to start if required.
		if (bMovingAWayFromStartPosition && CurrentPosition.y >= cAnchorPoint.y + fMovementWindowLength)
		{
			bMovingAWayFromStartPosition = false;
			return true;
		}

		// Test lower boundary limit passed and flip to moving away from start if required.
		if (!bMovingAWayFromStartPosition && CurrentPosition.y <= cAnchorPoint.y)
		{
			bMovingAWayFromStartPosition = true;
			return true;
		}
	}

	return false;
}

void CGCObjEnemy::SetFacingOrientation()
{

	if (k_bArtDefaultIsEnemyFacingRight)
	{
		SetFlippedX(!bMovingAWayFromStartPosition);
	}
	else
	{
		SetFlippedX(bMovingAWayFromStartPosition);
	}

}


void CGCObjEnemy::VOnUpdate(float fTimeStep)
{
	CGCObject::VOnUpdate(fTimeStep);

	if (bMovingAWayFromStartPosition)
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this class' public interface
