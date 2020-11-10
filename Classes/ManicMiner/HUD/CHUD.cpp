#include <string>

#include "CHUD.h"

#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "ManicMiner/Layers/CManicLayer.h"

#include "platform/CCPlatformMacros.h"

USING_NS_CC;

CHUD::CHUD( CManicLayer& cLayer )
	: m_pLivesLabel			( nullptr  )
	, m_pScoreLabel			( nullptr  )
	, m_pHighScoreLabel		( nullptr  )
	, m_pglOwnerGameLayer	( &cLayer  )
{
	m_pLivesLabel = Label::createWithTTF( "LIVES: " + std::to_string( 3 ), "fonts/arial.ttf", 20 );
	m_pLivesLabel->setGlobalZOrder( 3.f );
	m_pLivesLabel->setPosition( cocos2d::Vec2( 50.f, 1030.f ) );

	m_pglOwnerGameLayer->addChild( m_pLivesLabel, 2 );

	
	m_pScoreLabel = Label::createWithTTF( "SCORE: " + std::to_string( 0 ), "fonts/arial.ttf", 20 );
	m_pScoreLabel->setGlobalZOrder( 3.f );
	m_pScoreLabel->setPosition( cocos2d::Vec2( 300.f, 1030.f ) );

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

void CHUD::UpdateHighScore(int highScore)
{
	m_pglOwnerGameLayer->removeChild( m_pHighScoreLabel );

	m_pHighScoreLabel = Label::createWithTTF( "HIGHSCORE: " + std::to_string( highScore ), "fonts/arial.ttf", 20 );
	m_pHighScoreLabel->setGlobalZOrder( 3.f );
	m_pHighScoreLabel->setPosition( cocos2d::Vec2( 600.f, 1030.f ) );

	m_pglOwnerGameLayer->addChild( m_pHighScoreLabel, 2 );
}