#include "CMLCentralCavern.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ManicMiner/Enemy/CentralCavern/GCObjGroupEnemyCentralCavern.h"
#include "ManicMiner/Hazards/CentralCavern/GCObjGroupHazardCentralCavern.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/Platforms/CentralCavern/CObjCCGroupPlatform.h"

CMLCentralCavern::CMLCentralCavern( )
	: CManicLayer( )
	, m_pcGCGroupEnemyCentralCavern(nullptr)
	, m_pcGCGroupHazardCentralCavern(nullptr)
	, m_pcCollectibleGroup(nullptr)
{}

CMLCentralCavern::~CMLCentralCavern()
{}

void CMLCentralCavern::VOnCreate( void )
{
	CManicLayer::VOnCreate();


	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	cocos2d::Vec2	v2NewStart( ( origin.x + ( visibleSize.width * 0.5f ) ), ( origin.y + ( visibleSize.height * 0.33f ) ) );


	// Edit Custom Layout
	GetPlayer().SetResetPosition( v2NewStart );


	// Enemies for Central Cavern
	m_pcGCGroupEnemyCentralCavern = new CGCObjGroupEnemyCentralCavern();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupEnemyCentralCavern );

	// Hazards for Central Cavern
	m_pcGCGroupHazardCentralCavern = new CGCObjGroupHazardCentralCavern();
	CGCObjectManager::ObjectGroupRegister(m_pcGCGroupHazardCentralCavern);

	// Platforms for Central Cavern
	m_pcGroupPlatformCentralCavern = new CObjCCGroupPlatform();
	CGCObjectManager::ObjectGroupRegister(m_pcGroupPlatformCentralCavern);

	// Collectibles Group
	m_pcCollectibleGroup = new CCollectiblesGroup( *this);
	CGCObjectManager::ObjectGroupRegister( m_pcCollectibleGroup );
}

void CMLCentralCavern::VOnDestroy(void)
{
	CGCObjectManager::ObjectGroupUnRegister(m_pcGCGroupEnemyCentralCavern);
	safeDelete( m_pcGCGroupEnemyCentralCavern );

	CGCObjectManager::ObjectGroupUnRegister(m_pcGCGroupHazardCentralCavern);
	safeDelete(m_pcGCGroupHazardCentralCavern);

	CGCObjectManager::ObjectGroupUnRegister(m_pcCollectibleGroup);
	safeDelete( m_pcCollectibleGroup );

	CGCObjectManager::ObjectGroupUnRegister(m_pcGroupPlatformCentralCavern);
	safeDelete( m_pcGroupPlatformCentralCavern );

	// Call base class last
	CManicLayer::VOnDestroy();
}