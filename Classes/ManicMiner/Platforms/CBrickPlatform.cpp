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
	, m_bIsUnderPlayer( false )
{
	// Set PlatformType to Brick
	m_ePlatformType = EPlatformType::Brick;
}

void CBrickPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();

	GetSprite()->setVisible(false);
}

void CBrickPlatform::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams,
	cocos2d::Vec2 v2InitialPosition)
{
	const CGCFactoryCreationParams* pParamsToPassToBaseClass = &rCreationParams;

	//Fetch a pointer into the OGMO Xml edtior element containing the data.
	const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();

	if (nullptr != pCurrentObjectXmlData)
	{
		// Read in the custom plist and shape
		const tinyxml2::XMLAttribute* pCustomPlistPath = pCurrentObjectXmlData->FindAttribute("CustomPlist");
		const tinyxml2::XMLAttribute* pCustomShapePath = pCurrentObjectXmlData->FindAttribute("CustomShape");

		if ((nullptr != pCustomPlistPath)
			&& (0 != strlen(pCustomPlistPath->Value())))
		{
			m_pcCustomCreationParams = std::make_unique< CGCFactoryCreationParams >(rCreationParams.strClassName.c_str(),
				pCustomPlistPath->Value(),
				pCustomShapePath->Value(),
				rCreationParams.eB2dBody_BodyType,
				rCreationParams.bB2dBody_FixedRotation);

			pParamsToPassToBaseClass = m_pcCustomCreationParams.get();
		}
	}
	
	CPlatform::VHandleFactoryParams(*pParamsToPassToBaseClass, v2InitialPosition);
}

void CBrickPlatform::VOnReset()
{
	CPlatform::VOnReset();
	m_bCollisionEnabled = true;
	m_bTriggersHardContactEvent = true;
}

bool CBrickPlatform::GetIsUnderPlayer() const
{
	return m_bIsUnderPlayer;
}

void CBrickPlatform::SetIsUnderPlayer( const bool bIsUnderPlayer )
{
	m_bIsUnderPlayer = bIsUnderPlayer;
}
