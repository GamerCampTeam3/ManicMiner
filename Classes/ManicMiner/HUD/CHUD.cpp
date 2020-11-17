#include <string>

#include "CHUD.h"

#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "ManicMiner/Layers/CManicLayer.h"

#include "platform/CCPlatformMacros.h"

USING_NS_CC;

CHUD::CHUD( CManicLayer& cLayer, cocos2d::Point pOrigin, cocos2d::Size visibleSize )
	: m_pLivesLabel			( nullptr		)
	, m_pScoreLabel			( nullptr		)
	, m_pHighScoreLabel		( nullptr		)
	, m_pLevelName			( nullptr		)
	, m_pglOwnerGameLayer	( &cLayer		)
	, m_pOrigin				( pOrigin		)
	, m_visibleSize			( visibleSize	)
{
	// First draw of the HUD
	// 1) Lives
	m_pLivesLabel = Label::createWithTTF( "LIVES: " + std::to_string( 3 ), "fonts/arial.ttf", 20 );
	m_pLivesLabel->setGlobalZOrder( 3.f );
	m_pLivesLabel->setPosition( Vec2( (m_pOrigin).x + 800.f, ((m_pOrigin).y + (m_visibleSize).height) - 40 ) );
	m_pglOwnerGameLayer->addChild( m_pLivesLabel, 2 );

	// 2) Score
	m_pScoreLabel = Label::createWithTTF( "SCORE: " + std::to_string( 0 ), "fonts/arial.ttf", 20 );
	m_pScoreLabel->setGlobalZOrder( 3.f );
	m_pScoreLabel->setPosition( Vec2( (m_pOrigin).x + 1400.0f, ((m_pOrigin).y + (m_visibleSize).height) - 92 ) );
	m_pglOwnerGameLayer->addChild( m_pScoreLabel, 2 );
}

void CHUD::UpdateLevelName(const std::string szLevelName)
{
	m_pLevelName = Label::createWithTTF( szLevelName, "fonts/arial.ttf", 40 );
	m_pLevelName->setGlobalZOrder( 3.f );	
	m_pLevelName->setPosition( Vec2( (m_pOrigin).x + 1520.f, ((m_pOrigin).y + (m_visibleSize).height) - 60 ) );

	m_pglOwnerGameLayer->addChild( m_pLevelName, 2 );
}

void CHUD::FlushText()
{
	m_pglOwnerGameLayer->removeChild( m_pScoreLabel );
	m_pglOwnerGameLayer->removeChild( m_pLivesLabel );
	m_pglOwnerGameLayer->removeChild( m_pHighScoreLabel );
	m_pglOwnerGameLayer->removeChild( m_pLevelName );
}


void CHUD::UpdateScore(int score)
{
	DrawElement( m_pScoreLabel, "SCORE: ", score, Vec2( (m_pOrigin).x + 1400.0f, ((m_pOrigin).y + (m_visibleSize).height) - 92 ) );
}

void CHUD::UpdateLives(int lives)
{
	DrawElement( m_pLivesLabel, "LIVES: ", lives, Vec2( (m_pOrigin).x + 800.f, ((m_pOrigin).y + (m_visibleSize).height) - 40 ) );
}

void CHUD::UpdateHighScore( int highScore )
{
	DrawElement( m_pHighScoreLabel, "HIGHSCORE: ", highScore, Vec2( (m_pOrigin).x + 1680.f, ((m_pOrigin).y + (m_visibleSize).height) - 92 ) );
}

void CHUD::DrawElement( cocos2d::Label* label, char* labelText, int labelValue, cocos2d::Vec2 labelPos) const
{
	if (m_pglOwnerGameLayer != nullptr)
	{
		m_pglOwnerGameLayer->removeChild( label );

		label = Label::createWithTTF( labelText + std::to_string( labelValue ), "fonts/arial.ttf", 20 );
		label->setGlobalZOrder( 3.f );
		label->setPosition( labelPos );

		m_pglOwnerGameLayer->addChild( label, 2 );
	}
}
