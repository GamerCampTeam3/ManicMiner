#include "ManicMiner/Doors/CDoor.h"

#include <string.h>
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"




#ifndef TINYXML2_INCLUDED
	#include "external\tinyxml2\tinyxml2.h"
#endif

#ifndef _GCLEVELLOADER_OGMO_H_
	#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

USING_NS_CC;

GCFACTORY_IMPLEMENT_CREATEABLECLASS( CDoor );


//// Constructor that initializes all values.
//// The references are fed in by the door managers.
//CDoor::CDoor(CManicLayer& cLayer, CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition)
//	: CGCObjSpritePhysics( GetGCTypeIDOf( CDoor ) )
//	, m_cManicLayer						( cLayer )
//	, m_FactoryCreationParams			( CreationParams )
//	, m_v2ResetPosition					( ResetPosition )
//{	
//}

CDoor::CDoor()
	: CGCObjSpritePhysics( GetGCTypeIDOf ( CDoor) )
	, m_pCustomCreationParams( nullptr )
{

}

void CDoor::VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition )
{
	const CGCFactoryCreationParams* pParamsToPassToBaseClass = &rCreationParams;
	
	const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();
	
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
	
	CGCObjSpritePhysics::VHandleFactoryParams( (*pParamsToPassToBaseClass), v2InitialPosition );
}









// Sets the sprites as well as reset position
void CDoor::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();
	const CGCFactoryCreationParams* const pcCreateParams = GetFactoryCreationParams();
}

// Calls parent Reset 
void CDoor::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
}
