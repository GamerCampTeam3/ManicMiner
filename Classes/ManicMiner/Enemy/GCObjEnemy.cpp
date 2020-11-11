////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjEnemy.h"

#ifndef TINYXML2_INCLUDED
    #include "external\tinyxml2\tinyxml2.h"
#endif

#ifndef _GCLEVELLOADER_OGMO_H_
    #include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

USING_NS_CC;

GCFACTORY_IMPLEMENT_CREATEABLECLASS( CGCObjEnemy );

//////////////////////////////////////////////////////////////////////////
// Constructor (module 1)
//////////////////////////////////////////////////////////////////////////
/*
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

	pAnimation = nullptr;
}
*/

CGCObjEnemy::CGCObjEnemy()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjEnemy))
	, m_pCustomCreationParams(nullptr)
{
	m_fMoveDelta = 0.0;
}

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjEnemy::~CGCObjEnemy()
{
	
}

//////////////////////////////////////////////////////////////////////////
// This function initialises the enemies velocity, reset position and 
// initial facing direction.
//////////////////////////////////////////////////////////////////////////
// virtual function
//////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::VOnResourceAcquire( void )
{

    // Removed maco call so the reference m_rFactorCreationParams could be passed 
	// into VHandleFactoryParms.  Pending module 2 framework his may be done differently.
	//IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjEnemy );    
	//VHandleFactoryParams(m_rFactoryCreationParams, GetResetPosition());

	// Call base class verion.
	CGCObjSpritePhysics::VOnResourceAcquire();

	const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();

	/////////////////////////////////////////////
	// Set up Animations.
	// For module 2:
	// Commented out until animations available for Duck enemy.
	// std::string m_pszPlist = pcCreateParams->strPlistFile;
	// Note m_pszAnimation is sourced from the data file so not set here.

	//cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist(m_pszPlist);
	//pAnimation = GCCocosHelpers::CreateAnimation(rdictPList, m_pszAnimation);
	//pAnimation->retain();
	   
    //RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));

	//////////////////////////
	

	


	m_cTotalVelocity = Vec2::ZERO;
	



	Vec2 AnchorPointAndOffset = GetResetPosition();
	m_cAnchorPoint = GetResetPosition();


	if (EnemyTypes::EMovementAxis::EMovementAxis_Horizontal == m_eMovementAxis)
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



	//Alternative movement solution.
	m_cDest = Vec2(m_cAnchorPoint.x+30.0, m_cAnchorPoint.y);


	m_cCurrentPos = m_cAnchorPoint;

}

//////////////////////////////////////////////////////////////////////////

void  CGCObjEnemy::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition)
{

	const CGCFactoryCreationParams* pParamsToPassToBaseClass = &rCreationParams;



	
	//Fetch a pointer into the OGMO Xml edtior element containing the data.
	const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();

	if (nullptr != pCurrentObjectXmlData)
	{
		// Read in all Enemy specific values defined for the Enemy entity in the OGMO file and assign to member values.

		const tinyxml2::XMLAttribute* pAnimationName = pCurrentObjectXmlData->FindAttribute("AnimationName");
		CCLOG((nullptr == pAnimationName) ? "AnimationName not found for Enemy!" : pAnimationName->Value());
		m_pszAnimation = pAnimationName->Value();
		
		const tinyxml2::XMLAttribute* pMovementRange = pCurrentObjectXmlData->FindAttribute("MovementRange");
		CCLOG((nullptr == pMovementRange) ? "MovementRange not found for Enemy!" : pMovementRange->Value());
		m_fMovementWindowLength = pMovementRange->FloatValue();

				
		const tinyxml2::XMLAttribute* pInitialDistanceFromAnchor = pCurrentObjectXmlData->FindAttribute("InitialDistanceFromAnchor");
		CCLOG((nullptr == pInitialDistanceFromAnchor) ? "InitialDistanceFromAnchor not found for Enemy!" : pInitialDistanceFromAnchor->Value());
		m_fInitialDistanceFromAnchor = pInitialDistanceFromAnchor->FloatValue();


		const tinyxml2::XMLAttribute* pMovingAwayFromAnchorPoint = pCurrentObjectXmlData->FindAttribute("MovingAwayFromAnchorPoint");
		CCLOG((nullptr == pMovingAwayFromAnchorPoint) ? "MovingAwayFromAnchorPoint not found for Enemy!" : pMovingAwayFromAnchorPoint->Value());
		m_bMovingAwayFromAnchorPoint = pMovingAwayFromAnchorPoint->BoolValue();
		
		const tinyxml2::XMLAttribute* pSpeed = pCurrentObjectXmlData->FindAttribute("Speed");
		CCLOG((nullptr == pMovingAwayFromAnchorPoint) ? "Speed not found for Enemy!" : pSpeed->Value());
		m_fSpeed = pSpeed->FloatValue();
				
		const tinyxml2::XMLAttribute* pSpriteIsFlippable = pCurrentObjectXmlData->FindAttribute("SpriteIsFlippable");
		CCLOG((nullptr == pMovingAwayFromAnchorPoint) ? "SpriteIsFlippable not found for Enemy!" : pSpriteIsFlippable->Value());
		m_bSpriteIsFlippable = pSpriteIsFlippable->BoolValue();
			
		const tinyxml2::XMLAttribute* pMovementAxis = pCurrentObjectXmlData->FindAttribute("MovementAxis");
		CCLOG((nullptr == pMovementAxis) ? "SpriteIsFlippable not found for Enemy!" : pMovementAxis->Value());
		
		if (!strcmp(pMovementAxis->Value(),"Horizontal"))
		{
			m_eMovementAxis = EnemyTypes::EMovementAxis::EMovementAxis_Horizontal;
		}
		else
		{
			m_eMovementAxis = EnemyTypes::EMovementAxis::EMovementAxis_Vertical;
		}


		const tinyxml2::XMLAttribute* pCustomPlistPath = pCurrentObjectXmlData->FindAttribute("CustomPlist");


		if ((nullptr != pCustomPlistPath)
			&& (0 != strlen(pCustomPlistPath->Value())))
		{
			m_pCustomCreationParams = std::make_unique< CGCFactoryCreationParams >(rCreationParams.strClassName.c_str(),
				pCustomPlistPath->Value(),
				rCreationParams.strPhysicsShape.c_str(),
				rCreationParams.eB2dBody_BodyType,
				rCreationParams.bB2dBody_FixedRotation);

			pParamsToPassToBaseClass = m_pCustomCreationParams.get();
		}

	}

	// Call base class version 	
	CGCObjSpritePhysics::VHandleFactoryParams((*pParamsToPassToBaseClass), v2InitialPosition);

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
	if (m_bMovingAwayFromAnchorPoint && (fCurrentPosition >= fAnchorPoint + fMovementWindowLength))
	{
		m_bMovingAwayFromAnchorPoint = false;
		bReturnResult = true;
	}
	// Test left or lower side boundary limit passed and flip to moving away from start if required.
	else if (!m_bMovingAwayFromAnchorPoint && (fCurrentPosition <= fAnchorPoint))
	{
		m_bMovingAwayFromAnchorPoint = true;
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
	if (EnemyTypes::EMovementAxis::EMovementAxis_Horizontal == m_eMovementAxis)
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
	   	 
	/*cocos2d::Vec2 diff =  m_cAnchorPoint - m_cDest;
    float mag = diff.length();

    m_fMoveDelta = m_fMoveDelta + fTimeStep;
	
	//m_cCurrentPos.lerp(m_cDest, fTimeStep);
	//m_cCurrentPos.x = m_cCurrentPos.x + fTimeStep;

	//MoveToPixelPosition(m_cCurrentPos);


	*/



	if (m_bMovingAwayFromAnchorPoint)
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
		if (EnemyTypes::EMovementAxis::EMovementAxis_Horizontal == m_eMovementAxis && m_bSpriteIsFlippable)
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
	SetFlippedX((k_bArtDefaultIsEnemyFacingRight && !m_bMovingAwayFromAnchorPoint) || (!k_bArtDefaultIsEnemyFacingRight && m_bMovingAwayFromAnchorPoint));
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
		if (EnemyTypes::EMovementAxis::EMovementAxis_Horizontal == m_eMovementAxis && m_bSpriteIsFlippable)
		{
			SetFlippedX(!m_bMovingAwayFromAnchorPoint);
		}
		m_bMovingAwayFromAnchorPoint = !m_bMovingAwayFromAnchorPoint;
	}
	// Latch the flag to stop the enemy constantly flipping during the collisions duration.
	m_bBounceCollisionDisabled = true;
}
