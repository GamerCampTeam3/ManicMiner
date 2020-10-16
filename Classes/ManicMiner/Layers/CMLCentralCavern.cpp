#include "CMLCentralCavern.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ManicMiner/Enemy/GCObjGroupEnemyCentralCavern.h"

CMLCentralCavern::CMLCentralCavern( )
	: CManicLayer( )
	, m_pcGCGroupEnemyCentralCavern(nullptr)
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
	CGCObjectManager::ObjectGroupRegister(m_pcGCGroupEnemyCentralCavern);

	m_pcGCGroupEnemyCentralCavern->SetFormationOrigin(origin);
}

void CMLCentralCavern::VOnDestroy(void)
{
	// Call base class first.
	CManicLayer::VOnDestroy();

	CGCObjectManager::ObjectGroupUnRegister(m_pcGCGroupEnemyCentralCavern);
	delete m_pcGCGroupEnemyCentralCavern;
	m_pcGCGroupEnemyCentralCavern = nullptr;

}