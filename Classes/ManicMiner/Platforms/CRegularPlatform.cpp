////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CREGULARPLATFORM_H_
#include "CRegularPlatform.h"
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

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CRegularPlatform);

CRegularPlatform::CRegularPlatform()
	: CPlatform()
{
	m_ePlatformType = EPlatformType::Regular;
}

void CRegularPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();
}

void CRegularPlatform::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams,
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

void CRegularPlatform::VOnReset()
{
	CPlatform::VOnReset();
}
