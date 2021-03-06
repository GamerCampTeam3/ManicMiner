#include "ManicMiner/Collectible/CCollectible.h"


#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "ManicMiner/AudioHelper/ManicAudio.h"

#include <random>
#include <chrono>

#ifndef TINYXML2_INCLUDED
#include "external\tinyxml2\tinyxml2.h"
#endif

#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

USING_NS_CC;

GCFACTORY_IMPLEMENT_CREATEABLECLASS( CCollectible );


CCollectible::CCollectible()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CCollectible ) )
	, m_pCustomCreationParams	( nullptr )
	, m_bHasBeenCollected		( false )
	, m_fSpinDirection			( 0		) 
	, m_fSpinSpeed				( 0		)
{
	// Seed with time for pseudo randomness
	std::srand( timeGetTime() );
	// Then we get a random value between to act as speed
	m_fSpinSpeed = random( 0.f, m_kfMaxSpinSpeed );
	
	// Finally we get a random float between -1 and 1
	// If it's above 0, it we set it to 1, otherwise -1
	// We will use that to multiply the rotation value.
	SetDirection();
}

void CCollectible::SetDirection()
{
	m_fSpinDirection = rand_minus1_1();
	
	if (m_fSpinDirection > 0)
	{
		m_fSpinDirection = 1.0f;
	}
	else
	{
		m_fSpinDirection = -1.0f;
	}
}

void CCollectible::VOnResourceAcquire( void )
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	if (m_pszAnimation.length() > 0)
	{

		const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();

		std::string m_pszPlist = pcCreateParams->strPlistFile;

		// Note m_pszAnimation is sourced from the data file so not set here.
		cocos2d::ValueMap& rdictPList = GCCocosHelpers::CreateDictionaryFromPlist( m_pszPlist );
		pAnimation = GCCocosHelpers::CreateAnimation( rdictPList, m_pszAnimation );
		pAnimation->retain();

		RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );
	}
}


void CCollectible::VOnResourceRelease()
{
	CGCObjSpritePhysics::VOnResourceRelease();

	if (m_pszAnimation.length() > 0)
	{
		pAnimation->release();
		pAnimation = nullptr;
	}
}

void CCollectible::VOnUpdate(float fTimeStep)
{
	// Original Rotation + flat speed * delta time * speed multiplier * direction (1 = nothing, -1 = opposite);
	const float fNewRotation = GetSpriteRotation() + m_fSpinSpeed * fTimeStep * m_kfSpinMultiplier * m_fSpinDirection;
	SetSpriteRotation( fNewRotation );	
}


void CCollectible::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	m_bHasBeenCollected = false;
}

/// <summary>
///  Overriden function from the CGCObjSpritePhysics.
///  Thanks to Dave's implementation, this allows for Custom plists to be loaded in
///  without needing to make multiple classes.
/// </summary>
/// <param name="rCreationParams">	This will set the texture, shape and physics of the object.</param>
/// <param name="v2InitialPosition">		The position that the object will spawn in.</param>
void CCollectible::VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition )
{
	const CGCFactoryCreationParams* pParamsToPassToBaseClass = &rCreationParams;

	//Fetch a pointer into the OGMO Xml edtior element containing the data.
	const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();

	// Read in the custom plist
	if (nullptr != pCurrentObjectXmlData)
	{
		
		const tinyxml2::XMLAttribute* pAnimationName = pCurrentObjectXmlData->FindAttribute( "AnimationName" );
		if (pAnimationName != nullptr)
		{
			m_pszAnimation = pAnimationName->Value();
		}
		
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
	CGCObjSpritePhysics::VHandleFactoryParams( (*pParamsToPassToBaseClass), v2InitialPosition );
}

/// Main Interact Event
void CCollectible::InteractEvent()
{
	if (!m_bHasBeenCollected)
	{
		PlaySoundEffect( ESoundEffectName::KeyCollected );

		CGCObjectManager::ObjectKill( this );

		m_bHasBeenCollected = true;
	}
}