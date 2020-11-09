#include <string>

#include "CHUD.h"

#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"
#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Player/CPlayer.h"

#include "platform/CCPlatformMacros.h"

USING_NS_CC;

CHUD::CHUD( CPlayer& cPlayer, CManicLayer& cLayer, CCollectiblesGroup& collectiblesGroup )
	: CGCObject( GetGCTypeIDOf( CHUD ) )
	, m_pLivesLabel			( nullptr  )
	, m_pScoreLabel			( nullptr  )
	, m_pHighScoreLabel		( nullptr  )
	, m_pcPlayer			( &cPlayer )
	, m_pglOwnerGameLayer	( &cLayer  )
	, m_pCollectiblesGroup	( &collectiblesGroup )
{
}



void CHUD::VOnUpdate(float fTimeStep)
{
	 //UpdateLabel();
}


void CHUD::VOnReset()
{
	
}



void CHUD::UpdateLabel()
{
	// Step 1: clear text
	m_pglOwnerGameLayer->removeChild( m_pLivesLabel );
	m_pglOwnerGameLayer->removeChild( m_pScoreLabel );

	// Step 2: Update content
	m_pLivesLabel = Label::createWithTTF( "LIVES: " + std::to_string( m_pcPlayer->GetLives() ), "fonts/arial.ttf", 20 );
	m_pLivesLabel->setGlobalZOrder( 3.f );
	m_pLivesLabel->setPosition( cocos2d::Vec2( 50.f, 1030.f ) );

	m_pScoreLabel = Label::createWithTTF( "SCORE: " + std::to_string( m_pCollectiblesGroup->GetScore()), "fonts/arial.ttf", 20 );
	m_pScoreLabel->setGlobalZOrder( 3.f );
	m_pScoreLabel->setPosition( cocos2d::Vec2( 300.f, 1030.f ) );

	// Step 3: Draw text
	m_pglOwnerGameLayer->addChild( m_pLivesLabel, 2 );
	m_pglOwnerGameLayer->addChild( m_pScoreLabel, 2 );
}


void CHUD::UpdateScore(int score)
{
	m_pglOwnerGameLayer->removeChild( m_pScoreLabel );

	m_pScoreLabel = Label::createWithTTF( "SCORE: " + std::to_string( score ), "fonts/arial.ttf", 20 );
	m_pScoreLabel->setGlobalZOrder( 3.f );
	m_pScoreLabel->setPosition( cocos2d::Vec2( 300.f, 1030.f ) );

	m_pglOwnerGameLayer->addChild( m_pScoreLabel, 2 );

}

void CHUD::UpdateLives(int lives)
{
	m_pglOwnerGameLayer->removeChild( m_pLivesLabel );

	m_pLivesLabel = Label::createWithTTF( "LIVES: " + std::to_string( lives ), "fonts/arial.ttf", 20 );
	m_pLivesLabel->setGlobalZOrder( 3.f );
	m_pLivesLabel->setPosition( cocos2d::Vec2( 50.f, 1030.f ) );

	m_pglOwnerGameLayer->addChild( m_pLivesLabel, 2 );
}

void CHUD::UpdateHighScore(int highscore)
{
	m_pglOwnerGameLayer->removeChild( m_pHighScoreLabel );

	m_pHighScoreLabel = Label::createWithTTF( "HIGHSCORE: " + std::to_string( highscore ), "fonts/arial.ttf", 20 );
	m_pHighScoreLabel->setGlobalZOrder( 3.f );
	m_pHighScoreLabel->setPosition( cocos2d::Vec2( 600.f, 1030.f ) );

	m_pglOwnerGameLayer->addChild( m_pHighScoreLabel, 2 );
}