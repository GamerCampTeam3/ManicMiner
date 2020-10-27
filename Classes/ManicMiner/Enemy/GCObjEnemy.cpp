////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjEnemy.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CGCObjEnemy::CGCObjEnemy(const EnemyTypes::EMovementAxis EMovementAxisInput, const cocos2d::Vec2& rcAnchorPoint, const float fMovementRange, const float fInitialDistanceFromAnchor,
	const bool bMovingAwayFromAnchorPoint, const float fSpeed, const bool bSpriteIsFlippable, const EnemyTypes::EEnemyId eEnemyId, CGCFactoryCreationParams& rcFactoryCreationParamsInput)
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjEnemy))
	, m_eMovementAxis(EMovementAxisInput)
	, m_eEnemyId(eEnemyId)
	, m_cAnchorPoint(rcAnchorPoint)
	, m_fMovementWindowLength(fMovementRange)
	, m_fSpeed(fSpeed)
	, m_bMovingAWayFromAnchorPoint(bMovingAwayFromAnchorPoint)
	, m_rFactoryCreationParams(rcFactoryCreationParamsInput)
	, m_fInitialDistanceFromAnchor (fInitialDistanceFromAnchor)
	, m_bBounceCollisionDisabled(false)
	, m_bSpriteIsFlippable(bSpriteIsFlippable)
{
}

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjEnemy::~CGCObjEnemy()
{
}

//////////////////////////////////////////////////////////////////////////
// This function initialises the enemies direction of velocity. 
//////////////////////////////////////////////////////////////////////////
// virtual function
void CGCObjEnemy::VOnResourceAcquire( void )
{

    // Removed maco call so the reference m_rFactorCreationParams could be passed 
	// into VHandleFactoryParms.  Pending module 2 framework his may be done differently.
	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjEnemy );    
	VHandleFactoryParams(m_rFactoryCreationParams, GetResetPosition());

	CGCObjSpritePhysics::VOnResourceAcquire();

	m_cTotalVelocity = Vec2::ZERO;

	Vec2 AnchorPointAndOffset = m_cAnchorPoint;
	if (EnemyTypes::EMovementAxis::EMovementAxis_LeftRight == m_eMovementAxis)
	{
		m_cTotalVelocity.x = m_fSpeed;
		AnchorPointAndOffset.x += m_fInitialDistanceFromAnchor;
	}
	else
	{
		m_cTotalVelocity.y = m_fSpeed;
		AnchorPointAndOffset.y += m_fInitialDistanceFromAnchor;
	}

	SetResetPosition(AnchorPointAndOffset);

	SetFacingOrientation();
}

//////////////////////////////////////////////////////////////////////////
// This function is called when an enemy is resurected from the dead-list to the 
// live list.
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjEnemy::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}

//////////////////////////////////////////////////////////////////////////
// Function to detect if the current position of the enemy has passed a movement boundary.
// Returns true if a boundary has been reached or exceeded.
//////////////////////////////////////////////////////////////////////////
bool CGCObjEnemy::CheckForBoundaryReached(const float fCurrentPosition, const float fAnchorPoint, const float fMovementWindowLength)
{
	bool bReturnResult = false;

	// Test right or upper side boundary limit passed and flip to moving back to start if required.
	if (m_bMovingAWayFromAnchorPoint && (fCurrentPosition >= fAnchorPoint + fMovementWindowLength))
	{
		m_bMovingAWayFromAnchorPoint = false;
		bReturnResult = true;
	}
	// Test left or lower side boundary limit passed and flip to moving away from start if required.
	else if (!m_bMovingAWayFromAnchorPoint && (fCurrentPosition <= fAnchorPoint))
	{
		m_bMovingAWayFromAnchorPoint = true;
		bReturnResult = true;
	}
	return bReturnResult;
}

//////////////////////////////////////////////////////////////////////////
// Function to check the boundaries of the enemy movement window to detect a directional flip.
// Returns true if a directional flip was perfomed.
//////////////////////////////////////////////////////////////////////////
bool CGCObjEnemy::CheckForDirectionFlip()
{
	Vec2 CurrentPosition = GetSpritePosition();
	if (EnemyTypes::EMovementAxis::EMovementAxis_LeftRight == m_eMovementAxis)
	{
		return CheckForBoundaryReached(CurrentPosition.x, m_cAnchorPoint.x, m_fMovementWindowLength);
	}
	else
	{
		return CheckForBoundaryReached(CurrentPosition.y, m_cAnchorPoint.y, m_fMovementWindowLength);
	}
}

//////////////////////////////////////////////////////////////////////////
//Function to provide the frame update of this object
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjEnemy::VOnUpdate(float fTimeStep)
{
	// Call base class version first.
	CGCObject::VOnUpdate(fTimeStep);

	if (m_bMovingAWayFromAnchorPoint)
	{
		SetVelocity(m_cTotalVelocity);
	}
	else
	{
		SetVelocity(-m_cTotalVelocity);
	}
	
	if (CheckForDirectionFlip())
	{
		m_bBounceCollisionDisabled = false;
        // if the Enemy's direction has just flipped then need to change the facing orientation
		// for left/right orientation Enemy's.
		if (EnemyTypes::EMovementAxis::EMovementAxis_LeftRight == m_eMovementAxis && m_bSpriteIsFlippable)
		{
			SetFacingOrientation();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Function to set the facing orientation of the enemy sprite.
//////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::SetFacingOrientation()
{
	// Note the boolean k_bArtDefaultIsEnemyFacingRight is present
	// as the programmer art from the Gamer Camp framework faces by default to the
	// left.  However the facing direction delivered from the art team is facing right.

	const bool k_bArtDefaultIsEnemyFacingRight = false;
	SetFlippedX((k_bArtDefaultIsEnemyFacingRight && !m_bMovingAWayFromAnchorPoint) || (!k_bArtDefaultIsEnemyFacingRight && m_bMovingAWayFromAnchorPoint));
}
//////////////////////////////////////////////////////////////////////////
// Function to flip the enemies current direction of travel when it has collided with an object
// (note required only in the Kong Beast levels 8 and 12 when a wall is removed during gameplay).
// 
void CGCObjEnemy::BounceEnemyDirection()
{
	// If the enemy has just collided with an ojbect then this latching flag is set true to allow only
	// one flip of the sprite during collisions lifttime.   
	// The flag is reset when the enemy reaches the boundary at the other
	// side of its movement window.   This is a crude method but sufficient for the one edge case when
	// an enemy collides with an object which restricts the enemys moviement.  During gameplay the 
	// ojbect is removed therefore allowing greater movement of the enemy.

	if (!m_bBounceCollisionDisabled)
	{
		if (EnemyTypes::EMovementAxis::EMovementAxis_LeftRight == m_eMovementAxis && m_bSpriteIsFlippable)
		{
			SetFlippedX(!m_bMovingAWayFromAnchorPoint);
		}
		m_bMovingAWayFromAnchorPoint = !m_bMovingAWayFromAnchorPoint;
	}
	// Latch the flag to stop the enemy constantly flipping during the collisions duration.
	m_bBounceCollisionDisabled = true;
}
