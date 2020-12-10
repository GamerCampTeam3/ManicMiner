////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer December 2020 - Module 2
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
// Constructor
//////////////////////////////////////////////////////////////////////////
CGCObjEnemy::CGCObjEnemy()
	: CGCObjSpritePhysics(GetGCTypeIDOf(CGCObjEnemy))
	, m_pCustomCreationParams(nullptr)
{
	m_fMoveDelta							= m_kfZero;
	m_bTemporaryAnchorPositionActive		= false;
	m_bInitialiseToOne						= false;
	m_fPreviousXPos							= m_kfZero;
	m_bEnemyJustReceivedANewDestination		= false;
	m_cNewDestination						= cocos2d::Vec2::ZERO;
	m_cOriginalDestination					= cocos2d::Vec2::ZERO;
}

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
CGCObjEnemy::~CGCObjEnemy()
{
}

//////////////////////////////////////////////////////////////////////////
// This function initialises the enemies velocity, reset position and 
// initial facing direction.
//////////////////////////////////////////////////////////////////////////
// (Virtual function overriding immediate base class.)
//////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::VOnResourceAcquire( void )
{
	// Call base class version.
	CGCObjSpritePhysics::VOnResourceAcquire();

    // Only set up and run an animation if the data from the OGMO field has one specified.		   	 
	if (m_pszAnimation.length() > 0)
	{
		// Fetch the factory creation params and extract the plist for this object.
		const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();
		std::string m_pszPlist = pcCreateParams->strPlistFile;

		// Note m_pszAnimation is sourced from the data file so is set in VHandleFactoyParams.
		cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist(m_pszPlist);
		pAnimation = GCCocosHelpers::CreateAnimation(rdictPList, m_pszAnimation);
		pAnimation->retain();

		RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));
	}

	// Anchor point initialised to postion read from OGMO.
	m_cAnchorPoint = GetResetPosition();

	// Initailse the facing direction for the first traverse.
	SetFacingOrientation();

	// Special consideration required if this value > 0 which means the first traverse is shorter than the others,
	if (m_fInitialDistanceFromAnchor > 0)
	{

		// This flag will be checked at the end of the first traverse and used to restore normal walk window values.
		m_bTemporaryAnchorPositionActive = true;

		// Calculate the required arbitrary point along the movement window line as a Vector2 position.
		cocos2d::Vec2 cArbitraryPoint = m_cDest - m_cAnchorPoint;
		cArbitraryPoint.normalize();
		cArbitraryPoint = m_cAnchorPoint + (cArbitraryPoint * m_fInitialDistanceFromAnchor);

		// Set current and reset positions to this arbitrary point.
		m_cCurrentPos = cArbitraryPoint;
		SetResetPosition(cArbitraryPoint);

		// Need to store either the anchor point or the destination point in a temporary variable before overwriting with the 
		// just calculated arbitrary point position.  The original anchor/destination point will be re-instated at the end of the 
		// first traverse.
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

			// For this case need to initialise the LERP input to m_kfOne(1.0f) as we are starting from the Destination and moving to the Anchor point.
			m_fMoveDelta = m_kfOne;
			m_bInitialiseToOne = true;
		}
	}
	else
	{
		// Normal enemy patrol window size so nothing special to do here.
		m_cCurrentPos = m_cAnchorPoint;
		SetResetPosition(m_cAnchorPoint);
	}
	// Initialise the previous X position 
	m_fPreviousXPos = m_cCurrentPos.x;
}

//////////////////////////////////////////////////////////////////////////
// This function is called by the GC framework on level reset. 
//////////////////////////////////////////////////////////////////////////
// (Virtual function overriding immediate base class.)
//////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::VOnReset()
{
	// Call base class version first.
	CGCObjSpritePhysics::VOnReset();

	if (m_bInitialiseToOne)
	{
		// There is one edge case where the m_fMoveDelta needs initialising to 1.0f.  This is when we are starting at the destination and
    	// moving towards the anchor point.
		m_fMoveDelta = m_kfOne;
	}
	else
	{
		m_fMoveDelta = m_kfZero;
	}

	// In case the destination has been modifed (via a call to ModifyEnemyDestinationPoint), restore the original destination.
	m_cDest.x = m_cOriginalDestination.x;
	m_cDest.y = m_cOriginalDestination.y;

}

//////////////////////////////////////////////////////////////////////////
// This function is called by the GC framework when an enemy instance is generated by the object factory.
// The function is overriden here to provide an access point into the OGMO XML data entry row.
// Enemy specific data is read from the XML and placed into member variables.
// Some fields are custom (plist/shape) to the standard creation parameters and these are
// written over the standard fields.
//
//////////////////////////////////////////////////////////////////////////
// (Virtual function overriding immediate base class.)
//////////////////////////////////////////////////////////////////////////
void  CGCObjEnemy::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition)
{
	//Declare and fetch a pointer to provide access into the OGMO Xml edtior element currenctly being read.
	const CGCFactoryCreationParams* pParamsToPassToBaseClass = &rCreationParams;
	const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();

	if (nullptr != pCurrentObjectXmlData)
	{
		// Read in all Enemy specific values defined for the Enemy entity in the OGMO file and assign to member values.
		const tinyxml2::XMLAttribute* pAnimationName = pCurrentObjectXmlData->FindAttribute("AnimationName");
		CCLOG((nullptr == pAnimationName) ? "AnimationName not found for Enemy!" : pAnimationName->Value());
		m_pszAnimation = pAnimationName->Value();
				
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
				
		const tinyxml2::XMLAttribute* pName = pCurrentObjectXmlData->FindAttribute("Name");
		CCLOG((nullptr == pName) ? "Name not found for Enemy!" : pName->Value());
		m_psName = pName->Value();
		SetName(m_psName);

		// Modify destination axis inputs values just read as OGMO  origin is top left, but cocos2dx is bottom left.
		// (Use same method as GCLevelLoader_Ogmo uses).  This should be re-factored into a common operation call.
		cocos2d::Vec2 cLevelDimensions = CGCLevelLoader_Ogmo::GetLevelDimensions();
		Point origin = Director::getInstance()->getVisibleOrigin();
		cocos2d::Vec2 v2Origin(origin.x, origin.y);
		m_cDest = v2Origin + m_cDest;
		m_cDest.y = cLevelDimensions.y - m_cDest.y;
		m_cOriginalDestination.x = m_cDest.x;
		m_cOriginalDestination.y = m_cDest.y;

		// Read in the custom plist and shape fields to overwrite the standard parameter ones.
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
// (Virtual function overriding immediate base class.)
//////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::VOnResurrected( void )
{
	// Call base class version first.
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( m_kfZero );
}

//////////////////////////////////////////////////////////////////////////
// Function to provide the frame update of an enemy.
// - Updates m_cCurrentPos with the calculated LERP position during an enemies traverse 
//   of its movement window.
// - Handles all situations when the enemy arrives at its source or destination
//   anchor point.
//////////////////////////////////////////////////////////////////////////
// (Virtual function overriding CGCObject base class.)
//////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::VOnUpdate( float fTimeStep ) 
{
	// Call base class version first.
	CGCObject::VOnUpdate(fTimeStep);
	   	 
	// Calulate a vector to represent the movement window.
	cocos2d::Vec2 fVectorWindow = m_cAnchorPoint - m_cDest;
	
	// Calculate movement % increment amount as a function of the movment window length and the frame rate * speed modifier.
	// This value is the input into the LERP function to calculate m_cCurrentPos.
	float fLerpInput = ((fTimeStep * m_fSpeed) / fVectorWindow.length()) * 100.0f;

	// Add or subtract from movement amount depending on direction from anchor point
	if (m_bMovingAwayFromAnchorPoint)
	{
		m_fMoveDelta += fLerpInput;
	}
	else
	{
		m_fMoveDelta -= fLerpInput;
	}

	// Finally update the actual current position of the sprite by calling LERP.
	m_cCurrentPos = m_cAnchorPoint.lerp(m_cDest, m_fMoveDelta);

	if (m_fMoveDelta > m_kfZero && m_fMoveDelta < m_kfOne)
	{
		// This is the path taken during an enemies traverse inside its movement window.
		MoveToPixelPosition(m_cCurrentPos);
	}
	else
	{
		// This is the path taken when an enemy has arrived at either side of the movement window.

		// A TemporaryAnchorPositionActive signifies that the walk window now needs re-setting to its 'normal' size.
		if (m_bTemporaryAnchorPositionActive)
		{
			// need to restore normal walk cycle positions
			if (m_bMovingAwayFromAnchorPoint)
				m_cAnchorPoint = m_cTemporaryAnchorPosition;
			else
				m_cDest = m_cTemporaryAnchorPosition;

			// Clear this flag and this path will not be taken again as normal traversing window size.
			m_bTemporaryAnchorPositionActive = false;
		}

		// This is the situation when the movement window was updated with a new position (kong beast levels).
		// 
		if (m_bEnemyJustReceivedANewDestination)
		{
			// This path signifies that the enemy destination has been updated to a new position during this walk cycle.

			if (m_fMoveDelta < m_kfZero)
			{
				// At start side of 0-1 LERP calcuation so no need to worry about a visible step jump for the new window width.
			    // The new destination can just be slotted in and the LERP will recalculate correctly.
				m_cDest = m_cNewDestination;
			}
			else if (m_fMoveDelta > m_kfOne)
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
				m_fMoveDelta = m_kfZero;
			}
			// Clear this flag and this path will not be taken again until triggered.
			m_bEnemyJustReceivedANewDestination = false;
		}
		//-----------------------------------------------------------------------------
		else
		{
			// This path is not taken if m_bEnemyJustReceivedANewDestination is True as we don't want to perform a sprite flip
			// or clamp m_fMoveDelta.  This is because we are somewhere within the  movement window at this point.

			// Flip moving logic and facing orientation if required.
			m_bMovingAwayFromAnchorPoint = !m_bMovingAwayFromAnchorPoint;
			if (m_bSpriteIsFlippable)
			{
				SetFacingOrientation();
			}
			// Clamp value between 0 and 1 to avoid any 'stuck' situations occuring just outside of the 0 to 1 range.
			m_fMoveDelta = std::max(m_kfZero, std::min(m_fMoveDelta, m_kfOne));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Function to set the facing orientation of the enemy sprite.
// Compares the enemies current X coordinate with the X coordinate from the previous frame 
// to determine enemy direction, flipping the sprite accordingly using a base class function.
//////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::SetFacingOrientation()
{
	SetFlippedX(m_fPreviousXPos >= m_cCurrentPos.x);
}

//////////////////////////////////////////////////////////////////////////
// Function to perform any resource releasing tasks.
// (Virtual function overriding immediate base class.)
//////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::VOnResourceRelease()
{
	// call base class first.
	CGCObjSpritePhysics::VOnResourceRelease();

	// release an animation if one was specified in OGMO.
	if (m_pszAnimation.length() > 0)
	{
		pAnimation->release();
		pAnimation = nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Public function to allow game code to set the facing orientation of the enemy sprite.
///////////////////////////////////////////////////////////////////////////////////////
void CGCObjEnemy::ModifyEnemyDestinationPoint( cocos2d::Vec2& rcNewDestination )
{
	m_bEnemyJustReceivedANewDestination = true;
	m_cNewDestination = rcNewDestination;
}

