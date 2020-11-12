////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CBRICKPLATFORM_H_
#include "CBrickPlatform.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"


#ifndef TINYXML2_INCLUDED
#include "external\tinyxml2\tinyxml2.h"
#endif




#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CBrickPlatform);

CBrickPlatform::CBrickPlatform()
	: CPlatform()
{
	m_ePlatformType = EPlatformType::Brick;
}

void CBrickPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();
}

void CBrickPlatform::VOnReset()
{
	CPlatform::VOnReset();
	m_bCollisionEnabled = true;
	m_bTriggersHardContactEvent = true;
}