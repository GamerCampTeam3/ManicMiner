#include "CSwitch.h"
#include "ManicMiner/AudioHelper/ManicAudio.h"

#ifndef TINYXML2_INCLUDED
	#include "external\tinyxml2\tinyxml2.h"
#endif

#ifndef _GCLEVELLOADER_OGMO_H_
	#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif


GCFACTORY_IMPLEMENT_CREATEABLECLASS( CSwitch );

CSwitch::CSwitch()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CSwitch ) )
	, m_pCustomCreationParams			( nullptr )
	, m_bHasBeenCollected				( false )

{
}


/// <summary>
///  Overriden function from the CGCObjSpritePhysics.
///  Thanks to Dave's implementation, this allows for Custom plists to be loaded in
///  without needing to make multiple classes.
/// </summary>
/// <param name="rCreationParams">	This will set the texture, shape and physics of the object.</param>
/// <param name="v2InitialPosition">		The position that the object will spawn in.</param>
void CSwitch::VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition )
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
		}
	}

	// Call base class version 	
	CGCObjSpritePhysics::VHandleFactoryParams( *pParamsToPassToBaseClass, v2InitialPosition );
}


void CSwitch::VOnResourceAcquire( void )
{
	CGCObjSpritePhysics::VOnResourceAcquire();
}

void CSwitch::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	SetFlippedX( false );
	m_bHasBeenCollected = false;
}

// Main Interact Event
void CSwitch::InteractEvent()
{
	if (!m_bHasBeenCollected)
	{
		SetFlippedX( true );
		PlaySoundEffect( ESoundName::SwitchFlipped );
		
		// Stops collisions from happening again.
		m_bHasBeenCollected = true;
	}
}
