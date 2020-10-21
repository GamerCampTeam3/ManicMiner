#include "CMLTheColdRoom.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ManicMiner/Platforms/CentralCavern/CObjCCGroupPlatform.h"
#include "ManicMiner/Hazards/CentralCavern/GCObjGroupHazardCentralCavern.h"

CMLTheColdRoom::CMLTheColdRoom()
	: CManicLayer()
	//, m_pcGCGroupEnemyCentralCavern( nullptr )
{}

CMLTheColdRoom::~CMLTheColdRoom()
{}

void CMLTheColdRoom::VOnCreate( void )
{
	CManicLayer::VOnCreate();


	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	cocos2d::Vec2	v2NewStart( ( origin.x + ( visibleSize.width * 0.5f ) ), ( origin.y + ( visibleSize.height * 0.33f ) ) );


	// Edit Custom Layout
	GetPlayer().SetResetPosition( v2NewStart );


	//// Enemies for Central Cavern
	//m_pcGCGroupEnemyCentralCavern = new CGCObjGroupEnemyCentralCavern();
	//CGCObjectManager::ObjectGroupRegister( m_pcGCGroupEnemyCentralCavern );
	
	// Hazards for Central Cavern
	m_pcGCGroupHazardCentralCavern = new CGCObjGroupHazardCentralCavern();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupHazardCentralCavern );

	// Platforms for Central Cavern
	m_pcGroupPlatformCentralCavern = new CObjCCGroupPlatform();
	CGCObjectManager::ObjectGroupRegister( m_pcGroupPlatformCentralCavern );
}

void CMLTheColdRoom::VOnDestroy( void )
{
	CGCObjectManager::ObjectGroupUnRegister( m_pcGroupPlatformCentralCavern );
	delete m_pcGroupPlatformCentralCavern;
	m_pcGroupPlatformCentralCavern = nullptr;

	// Call base class last
	CManicLayer::VOnDestroy();
}