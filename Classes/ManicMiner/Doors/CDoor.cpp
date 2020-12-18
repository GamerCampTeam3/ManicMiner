#include "ManicMiner/Doors/CDoor.h"

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"


#ifndef TINYXML2_INCLUDED
	#include "external\tinyxml2\tinyxml2.h"
#endif

#ifndef _GCLEVELLOADER_OGMO_H_
	#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

USING_NS_CC;

GCFACTORY_IMPLEMENT_CREATEABLECLASS( CDoor );


// Base constructor
CDoor::CDoor()
	: CGCObjSpritePhysics( GetGCTypeIDOf ( CDoor) )
	, m_pCustomCreationParams			( nullptr  )
{
}

/// <summary>
///  Overriden function from the CGCObjSpritePhysics.
///  Thanks to Dave's implementation, this allows for Custom plists to be loaded in
///  without needing to make multiple classes.
/// </summary>
/// <param name="rCreationParams">	This will set the texture, shape and physics of the object.</param>
/// <param name="v2InitialPosition">		The position that the object will spawn in.</param>
void CDoor::VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition )
{
	const CGCFactoryCreationParams* pParamsToPassToBaseClass = &rCreationParams;

	//Fetch a pointer into the OGMO Xml edtior element containing the data.
	const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();

	// Read in the custom plist
	if (nullptr != pCurrentObjectXmlData)
	{

		const tinyxml2::XMLAttribute* pCustomPlistPath = pCurrentObjectXmlData->FindAttribute( "CustomPlist" );

		if ((nullptr != pCustomPlistPath)
			&& (0 != strlen( pCustomPlistPath->Value() )))
		{
			m_pCustomCreationParams = std::make_unique< CGCFactoryCreationParams >( rCreationParams.strClassName.c_str(),
				pCustomPlistPath->Value(),
				rCreationParams.strPhysicsShape.c_str(),
				rCreationParams.eB2dBody_BodyType,
				rCreationParams.bB2dBody_FixedRotation );

			pParamsToPassToBaseClass = m_pCustomCreationParams.get();
			SetName( "Door" );
		}
	}

	// Call base class version 	
	CGCObjSpritePhysics::VHandleFactoryParams( (*pParamsToPassToBaseClass), v2InitialPosition );
}

void CDoor::Open()
{
	cocos2d::Animation* pAnimation;
	const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();
	std::string m_pszPlist = pcCreateParams->strPlistFile;
	cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist( m_pszPlist );

	// Use this animation
	const char* m_pszAnimation = "Open";

	// Load it in and play
	pAnimation = GCCocosHelpers::CreateAnimation( rdictPList, m_pszAnimation );
	pAnimation->retain();

	pAnimation->setDelayPerUnit( 1.f/ 30.f );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );
	
	CCLOG( "Door opened" );
}


// Sets the sprites as well as reset position
void CDoor::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();
}

// Calls parent Reset 
void CDoor::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
}
