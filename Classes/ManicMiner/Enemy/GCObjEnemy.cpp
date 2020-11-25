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

CGCObjEnemy::CGCObjEnemy()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjEnemy))
	, m_pCustomCreationParams(nullptr)
{
	m_fMoveDelta = 0.0;
	m_bTemporaryAnchorPositionActive = false;
	m_bInitialiseToOne = false;
	m_fPreviousXPos = 0.0;
	m_bEnemyJustReceivedANewDestination = false;
	m_cNewDestination = cocos2d::Vec2::ZERO;
}

CGCObjEnemy::CGCObjEnemy(GCTypeID idDerivedType)
	: m_pCustomCreationParams(nullptr)
{
	m_fMoveDelta = 0.0;
	m_bTemporaryAnchorPositionActive = false;
	m_bInitialiseToOne = false;
	m_fPreviousXPos = 0.0;
	m_bEnemyJustReceivedANewDestination = false;
	m_cNewDestination = cocos2d::Vec2::ZERO;

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

	
		   	 
	if (m_pszAnimation.length() > 0)
	{
	
		const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();

		std::string m_pszPlist = pcCreateParams->strPlistFile;

		// Note m_pszAnimation is sourced from the data file so not set here.
		cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist(m_pszPlist);
		pAnimation = GCCocosHelpers::CreateAnimation(rdictPList, m_pszAnimation);
		pAnimation->retain();

		RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));


		// use below as data driven from OGMO to set animation speed if required?
		//pAnimation->setDelayPerUnit(0.0f);

	}
	
	m_cAnchorPoint = GetResetPosition();

	SetFacingOrientation();

	// Special consideration required if this value > 0 which means the first walk window path is shorter than the others.
	if (m_fInitialDistanceFromAnchor > 0)
	{

		// This flag will be checked at the end of the walk window and used to restore normal walk window values.
		m_bTemporaryAnchorPositionActive = true;

		// Calculate the required arbitrary point along the movement window as a Vector2 position.
		cocos2d::Vec2 cArbitraryPoint = m_cDest - m_cAnchorPoint;
		cArbitraryPoint.normalize();
		cArbitraryPoint = m_cAnchorPoint + (cArbitraryPoint * m_fInitialDistanceFromAnchor);

		// Set current and reset positions to this arbitrary point.
		m_cCurrentPos = cArbitraryPoint;
		SetResetPosition(cArbitraryPoint);

		// Need to store either the anchor point or the destination point in a temporary variable before overwriting with the 
		// just calculated arbitrary point position.  The original anchor/destination point will be re-instated at the end of the 
		// first walk window pass.
		if (m_bMovingAwayFromAnchorPoint)
		{

			m_cTemporaryAnchorPosition = m_cAnchorPoint;
			m_cAnchorPoint = cArbitraryPoint;

			// Results in:
			//           (Arbitrary start point)
			//   |-----------|---->--->---->--->-----|
			//               0  (LERP)               1			     
			//   			 Anc					Dest

		}
		else
		{
			m_cTemporaryAnchorPosition = m_cDest;
			m_cDest = cArbitraryPoint;

			// Results in:
			//           (Arbitrary start point)
			//   |-<---<---<-|-----------------------|
			//   0  (LERP)   1
			//   Anc        Dest

			// For this case need to initialise the LERP input to 1.0 as we are starting from the Destination and moving to the Anchor point.
			m_fMoveDelta = 1.0f;
			m_bInitialiseToOne = true;
		}

	}
	else
	{
		// Normal walk window size so nothing special to do here.
		m_cCurrentPos = m_cAnchorPoint;


		SetResetPosition(m_cAnchorPoint);

	}

	m_fPreviousXPos = m_cCurrentPos.x;

}

void CGCObjEnemy::VOnReset()
{
	// Call base class version first.
	CGCObjSpritePhysics::VOnReset();

	if (m_bInitialiseToOne)
	{
		m_fMoveDelta = 1.0f;
	}
	else
	{
		m_fMoveDelta = 0.0f;
	}

}





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
		
		//const tinyxml2::XMLAttribute* pMovementRange = pCurrentObjectXmlData->FindAttribute("MovementRange");
		//CCLOG((nullptr == pMovementRange) ? "MovementRange not found for Enemy!" : pMovementRange->Value());
		//m_fMovementWindowLength = pMovementRange->FloatValue();

				
		const tinyxml2::XMLAttribute* pInitialDistanceFromAnchor = pCurrentObjectXmlData->FindAttribute("InitialDistanceFromAnchor");
		CCLOG((nullptr == pInitialDistanceFromAnchor) ? "InitialDistanceFromAnchor not found for Enemy!" : pInitialDistanceFromAnchor->Value());
		m_fInitialDistanceFromAnchor = pInitialDistanceFromAnchor->FloatValue();


		const tinyxml2::XMLAttribute* pMovingAwayFromAnchorPoint = pCurrentObjectXmlData->FindAttribute("MovingAwayFromAnchorPoint");
		CCLOG((nullptr == pMovingAwayFromAnchorPoint) ? "MovingAwayFromAnchorPoint not found for Enemy!" : pMovingAwayFromAnchorPoint->Value());
		m_bMovingAwayFromAnchorPoint = pMovingAwayFromAnchorPoint->BoolValue();
		
		const tinyxml2::XMLAttribute* pSpeed = pCurrentObjectXmlData->FindAttribute("Speed");
		CCLOG((nullptr == pSpeed) ? "Speed not found for Enemy!" : pSpeed->Value());
		m_fSpeed = pSpeed->FloatValue();
				
		const tinyxml2::XMLAttribute* pSpriteIsFlippable = pCurrentObjectXmlData->FindAttribute("SpriteIsFlippable");
		CCLOG((nullptr == pMovingAwayFromAnchorPoint) ? "SpriteIsFlippable not found for Enemy!" : pSpriteIsFlippable->Value());
		m_bSpriteIsFlippable = pSpriteIsFlippable->BoolValue();
			



		
		const tinyxml2::XMLAttribute* pDestinationX = pCurrentObjectXmlData->FindAttribute("DestinationX");
		CCLOG((nullptr == pDestinationX) ? "DestX not found for Enemy!" : pDestinationX->Value());
		m_cDest.x = pDestinationX->FloatValue();

		
		const tinyxml2::XMLAttribute* pDestinationY = pCurrentObjectXmlData->FindAttribute("DestinationY");
		CCLOG((nullptr == pDestinationY) ? "DestX not found for Enemy!" : pDestinationY->Value());
		m_cDest.y = (pDestinationY->FloatValue());


		// Modify destination axis inputs values just read as OGMO  origin is top left, but cocos2dx is bottom left.
		// (Use same method as GCLevelLoader_Ogmo uses).

		cocos2d::Vec2 cLevelDimensions = CGCLevelLoader_Ogmo::GetLevelDimensions();
		Point origin = Director::getInstance()->getVisibleOrigin();
		cocos2d::Vec2 v2Origin(origin.x, origin.y);
		m_cDest = v2Origin + m_cDest;
		m_cDest.y = cLevelDimensions.y - m_cDest.y;

		// Read in the custom plist and shape
		const tinyxml2::XMLAttribute* pCustomPlistPath = pCurrentObjectXmlData->FindAttribute("CustomPlist");
		const tinyxml2::XMLAttribute* pCustomShapePath = pCurrentObjectXmlData->FindAttribute("CustomShape");
				
		if ((nullptr != pCustomPlistPath)
			&& (0 != strlen(pCustomPlistPath->Value())))
		{
			m_pCustomCreationParams = std::make_unique< CGCFactoryCreationParams >(rCreationParams.strClassName.c_str(),
				pCustomPlistPath->Value(),
				pCustomShapePath->Value(),
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
//Function to provide the frame update of this object
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjEnemy::VOnUpdate(float fTimeStep)
{
	// Call base class version first.
	CGCObject::VOnUpdate(fTimeStep);
	   	 



	// Calulate a vector to represent the movement window.
	cocos2d::Vec2 fVectorWindow = m_cAnchorPoint - m_cDest;


	// Calculate movement % increment amount as a function of the movment window length and the frame rate * speed modifier.
	// This value is the input into the LERP function to calculate m_cCurrentPos.
	float fLerpInput = ((fTimeStep * m_fSpeed) / fVectorWindow.length()) * 100.0f;


	if (m_bMovingAwayFromAnchorPoint)
	{
		m_fMoveDelta += fLerpInput;
	}
	else
	{
		m_fMoveDelta -= fLerpInput;
	}

	m_cCurrentPos = m_cAnchorPoint.lerp(m_cDest, m_fMoveDelta);





	if (m_fMoveDelta > 0.0f && m_fMoveDelta < 1.0f)
	{
		// This is the normal path taken during an enemies traverse of its movement window.
		MoveToPixelPosition(m_cCurrentPos);
	}
	else
	{
		// This is the path taken when an enemy has arrived at either side of the movement window.

		// A TemporaryAnchorPositionActive signifies that the walk window needs re-setting to its 'normal' size.
		if (m_bTemporaryAnchorPositionActive)
		{
			// need to restore normal walk cycle positions
			if (m_bMovingAwayFromAnchorPoint)
				m_cAnchorPoint = m_cTemporaryAnchorPosition;
			else
				m_cDest = m_cTemporaryAnchorPosition;

			// Clear this flag and this path will not be taken again.
			m_bTemporaryAnchorPositionActive = false;

		}


		// This is the situation when the movement window is updated with a new wider position (kong beast levels).
		// 

		if (m_bEnemyJustReceivedANewDestination)
		{
			// This path signifies that the enemy destination has been updated to a new position during this walk cycle.

			if (m_fMoveDelta < 0.0f)
			{
				// At start side of 0-1 LERP calcuation so no need to worry about a visible step jump for the new window width.
			    // The new destination can just be slotted in and the LERP will recalculate correctly.
				m_cDest = m_cNewDestination;

			}

			else if (m_fMoveDelta > 1.0f)
			{
				// For this case we need to calculate for the journey to the new destination point as a 'temporaryAnchorPositionActive',
				// using the same method as for if the initial distance from the anchor is set > 0 at startup.

				// Store current anchor point into the temporary variable and set a flag to signal this needs re-instating at the end of the current walk cycle.
				m_cTemporaryAnchorPosition = m_cAnchorPoint;
				m_bTemporaryAnchorPositionActive = true;

				// Set new anchor and destination points for the temporary movement window.
				m_cAnchorPoint = m_cCurrentPos;
				m_cDest = m_cNewDestination;
				
                // Set these values to ensure a standard walk cycle occurs for the temporary movement.
				m_bMovingAwayFromAnchorPoint = true;
				m_fMoveDelta = 0.0f;

			}

			// Clear this flag and this path will not be taken again until triggered.
			m_bEnemyJustReceivedANewDestination = false;

		}
		//-----------------------------------------------------------------------------
		else
		{
			// This path is not taken if m_bEnemyJustReceivedANewDestination is True as we don't want to perform a sprite flip
			// or clamp m_fMoveDelta for that situation.


			// Flip moving logic and facing orientation if required.
			m_bMovingAwayFromAnchorPoint = !m_bMovingAwayFromAnchorPoint;
			if (m_bSpriteIsFlippable)
			{
				SetFacingOrientation();
			}

			// Clamp value between 0 and 1 to avoid any 'stuck' situations occuring just outside of the 0 to 1 range.
			m_fMoveDelta = std::max(0.0f, std::min(m_fMoveDelta, 1.0f));
		}


	}
}

//////////////////////////////////////////////////////////////////////////
// Function to set the facing orientation of the enemy sprite.
//////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::SetFacingOrientation()
{
	SetFlippedX(m_fPreviousXPos >= m_cCurrentPos.x);
}


//////////////////////////////////////////////////////////////////////////

void CGCObjEnemy::VOnResourceRelease()
{
	// call base class first.
	CGCObjSpritePhysics::VOnResourceRelease();


	if (m_pszAnimation.length() > 0)
	{
		pAnimation->release();
		pAnimation = nullptr;

	}
}


//////////////////////////////////////////////////////////////////////////

void CGCObjEnemy::ModifyEnemyDestinationPoint(cocos2d::Vec2& rcNewDestination)
{
	
	m_bEnemyJustReceivedANewDestination = true;
	m_cNewDestination = rcNewDestination;


	// 
   //ModifyEnemyDestinationPoint(Vec2(1180.0, 475.0));

}

