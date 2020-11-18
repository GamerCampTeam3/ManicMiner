#include <string>

#include "CHUD.h"

#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "ManicMiner/Layers/CManicLayer.h"

#include "platform/CCPlatformMacros.h"


USING_NS_CC;

CHUD::CHUD( CManicLayer& cLayer, cocos2d::Point pOrigin, cocos2d::Size visibleSize )
	: m_pLivesLabel			( nullptr		)
	, m_pScoreLabel			( nullptr		)
	, m_pScoreTextLabel		( nullptr		)
	, m_pHighScoreLabel		( nullptr		)
	, m_pHighscoreTextLabel	( nullptr		)
	, m_pLevelName			( nullptr		)
	, m_pglOwnerGameLayer	( &cLayer		)
	, m_pointOrigin			( pOrigin		)
	, m_sizeVisible			( visibleSize	)
	, m_v2LivesPosition		(0,0		)
	, m_v2ScorePosition		(0,0		)
	, m_v2HighscorePosition	(0,0		)
	, m_v2LevelNamePosition	(0,0		)
{
	m_v2LivesPosition		= cocos2d::Vec2( (m_pointOrigin).x + 800.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 35 );
	m_v2ScorePosition		= cocos2d::Vec2( (m_pointOrigin).x + 1400.0f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 );
	m_v2HighscorePosition	= cocos2d::Vec2( (m_pointOrigin).x + 1640.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 );
	m_v2LevelNamePosition	= cocos2d::Vec2( (m_pointOrigin).x + 1520.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 40 );

	testX = 40.f;
	
	 cocos2d::Vec2 v2ScoreTextPos = cocos2d::Vec2( (m_pointOrigin).x + 1460.0f, ((m_pointOrigin).y + (m_sizeVisible).height) - 92 );
	 // 1) Lives
	 m_pLivesLabel = Label::createWithTTF( "LIVES: " + std::to_string( 3 ), "fonts/Pixeled.ttf", 20 );
	 m_pLivesLabel->setTextColor( cocos2d::Color4B( 165, 227, 251, 255 ) );
	 m_pLivesLabel->setGlobalZOrder( 3.f );
	 m_pLivesLabel->setPosition( m_v2LivesPosition );
	 m_pglOwnerGameLayer->addChild( m_pLivesLabel, 2 );
	// 
	// // 2) Score (text)
	// m_pScoreTextLabel = Label::createWithTTF( "SCORE: ", "fonts/arial.ttf", 20 );
	// m_pScoreTextLabel->setGlobalZOrder( 3.f );
	// m_pScoreTextLabel->setPosition( m_v2ScorePosition );
	// m_pglOwnerGameLayer->addChild( m_pScoreTextLabel, 2 );
	// 
	// 3) Score (value)
	m_pScoreLabel = Label::createWithTTF( std::to_string( 0 ), "fonts/Pixeled.ttf", 20 );
	m_pScoreLabel->setGlobalZOrder( 3.f );
	m_pScoreLabel->setTextColor( cocos2d::Color4B( 165, 227, 251, 255 ) );
	m_pScoreLabel->setPosition( v2ScoreTextPos );
	m_pglOwnerGameLayer->addChild( m_pScoreLabel, 2 );
}

void CHUD::UpdateLevelName(const std::string szLevelName) 
{
	char* pchr = _strdup( szLevelName.c_str() );

	DrawElement( m_pLivesLabel, pchr, -1, m_v2LevelNamePosition, 40);
}

void CHUD::FlushText() const
{

	// ->GetSprite()->retain();
	// ->RemoveFromParent();
	m_pglOwnerGameLayer->removeChild( m_pScoreLabel		);
	m_pglOwnerGameLayer->removeChild( m_pLivesLabel		);
	m_pglOwnerGameLayer->removeChild( m_pHighScoreLabel );
	m_pglOwnerGameLayer->removeChild( m_pLevelName		);
}

void CHUD::Init(std::string szLevelName, int iLives, int iScore, int iHighscore)
{
	cocos2d::Vec2 v2ScoreTextPos		= cocos2d::Vec2( (m_pointOrigin).x + 1460.0f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 );
	cocos2d::Vec2 v2HighscoreTextPos	= cocos2d::Vec2( (m_pointOrigin).x + 1780.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 );
	char* pchr = _strdup( szLevelName.c_str() );

	DrawElement( m_pScoreLabel, static_cast<char*>("SCORE:"), -1, m_v2ScorePosition, 20 );
	//DrawElement( m_pScoreTextLabel, static_cast<char*>(""), iScore, v2ScoreTextPos, 20 );

	DrawElement( m_pHighscoreTextLabel, static_cast<char*>("HIGHSCORE:"), -1, m_v2HighscorePosition, 20 );
	DrawElement( m_pHighScoreLabel, static_cast<char*>(""), iHighscore, v2HighscoreTextPos, 20 );

	//DrawElement( m_pLivesLabel, static_cast<char*>("LIVES: "), iLives, m_v2LivesPosition, 20 );
	
	DrawElement( m_pLevelName, pchr, -1, m_v2LevelNamePosition, 40 );


	
	// cocos2d::Vec2 testv2 = cocos2d::Vec2( x, 1020.f );
	//Vec2( (m_pOrigin).x + 620.f, ((m_pOrigin).y + (m_visibleSize).height) - 60 ) );
	
	
	for (int i = 0; i < 3; i++)
	{
		CGCObjSprite* mtest = new CGCObjSprite();
		
		static float x = 40.0f;
		cocos2d::Vec2 testv2 = cocos2d::Vec2( testX, 1025.f );

		mtest->CreateSprite( m_kpszPlistPlayerLifeFull );
		mtest->SetSpriteGlobalZOrder( 2.f );
		mtest->SetSpriteScale( 0.5f, 0.5f );
		mtest->SetResetPosition( testv2 );
		mtest->GetSprite()->setPosition( mtest->GetResetPosition() );
		mtest->SetParent( m_pglOwnerGameLayer );


		testX += 40.f;
	}
}

void CHUD::UpdateScore(int score) 
{
	cocos2d::Vec2 v2ScoreTextPos = cocos2d::Vec2( (m_pointOrigin).x + 1490.0f, ((m_pointOrigin).y + (m_sizeVisible).height) - 92 );
	cocos2d::Vec2 v2HighscoreTextPos = cocos2d::Vec2( (m_pointOrigin).x + 1780.f, ((m_pointOrigin).y + (m_sizeVisible).height) - 92 );
	
	m_pglOwnerGameLayer->removeChild( m_pScoreLabel );
	
	m_pScoreLabel = Label::createWithTTF( std::to_string( score ), "fonts/Pixeled.ttf", 20 );
	m_pScoreLabel->setTextColor( cocos2d::Color4B( 165, 227, 251, 255 ) );
	m_pScoreLabel->setGlobalZOrder( 3.f );
	m_pScoreLabel->setPosition( v2ScoreTextPos );
	
	m_pglOwnerGameLayer->addChild( m_pScoreLabel, 2 );
	
	//m_pglOwnerGameLayer->removeChild( m_pScoreLabel );
	//DrawElement( m_pScoreLabel, static_cast<char*>(""), score, v2ScoreTextPos, 20 );
}

void CHUD::UpdateLives(int lives) 
{
	m_pglOwnerGameLayer->removeChild( m_pLivesLabel );
	m_pLivesLabel = Label::createWithTTF( "LIVES: " + std::to_string( lives ), "fonts/Pixeled.ttf", 20 );
	m_pLivesLabel->setTextColor( cocos2d::Color4B( 165, 227, 251, 255 ) );
	m_pLivesLabel->setGlobalZOrder( 3.f );
	m_pLivesLabel->setPosition( m_v2LivesPosition );
	m_pglOwnerGameLayer->addChild( m_pLivesLabel, 2 );
}

void CHUD::UpdateHighScore( int highScore ) 
{
	DrawElement( m_pHighScoreLabel, static_cast<char*>("HIGHSCORE: "), highScore, m_v2HighscorePosition, 20 );
}

void CHUD::DrawElement( cocos2d::Label* label, char* labelText, int labelValue, cocos2d::Vec2 labelPos, float fFontSize) const
{
	if (m_pglOwnerGameLayer != nullptr)
	{
		if (label != nullptr)
		{
			m_pglOwnerGameLayer->removeChild( label );
		}

		if (labelValue != -1)
		{
			label = Label::createWithTTF( labelText + std::to_string( labelValue ), "fonts/Pixeled.ttf", fFontSize );
		}
		else
		{
			label = Label::createWithTTF( labelText, "fonts/Pixeled.ttf", fFontSize );
		}

		label->setTextColor( cocos2d::Color4B( 165, 227, 251, 255 ) );
		label->setGlobalZOrder( 3.f );
		label->setPosition( labelPos );

		m_pglOwnerGameLayer->addChild( label, 2 );
	}
}
