#include "CHUD.h"

#include "ManicMiner/Player/CPlayer.h"

#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"
#include "ManicMiner/Layers/CManicLayer.h"
#include "platform/CCPlatformMacros.h"
#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"

#include <string>

#include "ManicMiner/Helpers/Helpers.h"
USING_NS_CC;

CHUD::CHUD( CPlayer& cPlayer, CManicLayer& cLayer, CCollectiblesGroup& collectiblesGroup )
	: CGCObject( GetGCTypeIDOf( CHUD ) )
	, m_pLivesLabel(nullptr)
	, m_pScoreLabel(nullptr)
	, m_pcPlayer( &cPlayer )
	, m_pglOwnerGameLayer( &cLayer )
	, m_pCollectiblesGroup( &collectiblesGroup )
{

}



void CHUD::VOnUpdate(float fTimeStep)
{
	UpdateLabel();
}


void CHUD::VOnReset()
{
	
}



void CHUD::UpdateLabel()
{
	m_pglOwnerGameLayer->removeChild( m_pLivesLabel );
	m_pglOwnerGameLayer->removeChild( m_pScoreLabel );
	
	m_pLivesLabel = Label::createWithTTF( "LIVES: " + std::to_string( m_pcPlayer->GetLives() ), "fonts/arial.ttf", 20 );
	m_pLivesLabel->setGlobalZOrder( 3.f );
	m_pLivesLabel->setPosition( cocos2d::Vec2( 50.f, 1030.f ) );

	m_pScoreLabel = Label::createWithTTF( "SCORE: " + std::to_string( m_pCollectiblesGroup->GetScore()), "fonts/arial.ttf", 20 );
	m_pScoreLabel->setGlobalZOrder( 3.f );
	m_pScoreLabel->setPosition( cocos2d::Vec2( 300.f, 1030.f ) );

	m_pglOwnerGameLayer->addChild( m_pLivesLabel, 2 );
	m_pglOwnerGameLayer->addChild( m_pScoreLabel, 2 );
}

CHUD::~CHUD()
{
	m_pLivesLabel->release();
	delete m_pLivesLabel;
	m_pLivesLabel = nullptr;

	m_pScoreLabel->release();
	delete m_pScoreLabel;
	m_pScoreLabel = nullptr;
}

