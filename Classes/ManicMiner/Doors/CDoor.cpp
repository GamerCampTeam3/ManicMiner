#include "ManicMiner/Doors/CDoor.h"
#include "ManicMiner/Layers/CManicLayer.h"


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
{

}

void CDoor::VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition )
{



	CGCObjSpritePhysics::VHandleFactoryParams( rCreationParams, v2InitialPosition );
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
