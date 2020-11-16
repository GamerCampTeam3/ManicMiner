////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMOVINGPLATFORM_H_
#include "CMovingPlatform.h"
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

#include "ManicMiner/Enums/EPlayerMovement.h"

GCFACTORY_IMPLEMENT_CREATEABLECLASS(CMovingPlatform);

CMovingPlatform::CMovingPlatform()
	: CPlatform()
	, m_pcMovingAnim ( nullptr )
{
	m_ePlatformType = EPlatformType::Moving;
}

void CMovingPlatform::InitDirectionalLock(const EPlayerDirection eDirectionLock)
{
	m_eDirectionLock = eDirectionLock;
}

void CMovingPlatform::VOnResourceAcquire()
{
	CPlatform::VOnResourceAcquire();

	const char* pszAnim_Move = "MoveLeft";

	// Used to be able to have animation - no animation provided by the artist therefore feature was removed
	//cocos2d::ValueMap& rdictPlist = GCCocosHelpers::CreateDictionaryFromPlist(GetFactoryCreationParams()->strPlistFile);
	//m_pcMovingAnim = GCCocosHelpers::CreateAnimation(rdictPlist, pszAnim_Move);
	//RunAction(GCCocosHelpers::CreateAnimationActionLoop(m_pcMovingAnim));

	InitDirectionalLock(EPlayerDirection::Left);
}

void CMovingPlatform::VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams,
	cocos2d::Vec2 v2InitialPosition)
{
	const CGCFactoryCreationParams* pParamsToPasstoBaseClass = &rCreationParams;

	//Fetch a pointer into the OGMO Xml edtior element containing the data.
	const tinyxml2::XMLElement* pCurrentObjectXmlData = CGCLevelLoader_Ogmo::GetCurrentObjectXmlData();

	if (nullptr != pCurrentObjectXmlData)
	{
		const tinyxml2::XMLAttribute* pCustomPlistPath = pCurrentObjectXmlData->FindAttribute("CustomPlist");

		if ((nullptr != pCustomPlistPath)
			&& (0 != strlen(pCustomPlistPath->Value())))
		{
			m_pcCustomCreationParams = std::make_unique< CGCFactoryCreationParams >(rCreationParams.strClassName.c_str(),
				pCustomPlistPath->Value(),
				rCreationParams.strPhysicsShape.c_str(),
				rCreationParams.eB2dBody_BodyType,
				rCreationParams.bB2dBody_FixedRotation);

			pParamsToPasstoBaseClass = m_pcCustomCreationParams.get();
		}
	}

	CPlatform::VHandleFactoryParams(*pParamsToPasstoBaseClass, v2InitialPosition);
}

void CMovingPlatform::VOnReset()
{
	CPlatform::VOnReset();
}


const EPlayerDirection CMovingPlatform::GetDirectionLock()
{
	return m_eDirectionLock;
}