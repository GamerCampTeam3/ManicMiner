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
	, m_pointOrigin			( pOrigin		)
	, m_sizeVisible			( visibleSize	)
{
	// First draw of the HUD
	// 1) Lives
	DrawElement( m_pLivesLabel, static_cast<char*>("LIVES: "), 3, Vec2( (m_pointOrigin).x + 800.f, ((m_pointOrigin).y + (m_sizeVisible).height) - 40 ), 20 );
	DrawElement( m_pScoreLabel, static_cast<char*>("SCORE: "), 0, Vec2( (m_pointOrigin).x + 1400.0f, ((m_pointOrigin).y + (m_sizeVisible).height) - 92 ), 20 );
}

void CHUD::UpdateLevelName(const std::string szLevelName) const
{
	char* pchr = _strdup( szLevelName.c_str() );

	DrawElement( m_pLivesLabel, pchr, 0, Vec2( (m_pointOrigin).x + 1520.f, ((m_pointOrigin).y + (m_sizeVisible).height) - 60 ) , 40, true);
}

void CHUD::FlushText() const
{
	m_pglOwnerGameLayer->removeChild( m_pScoreLabel		);
	m_pglOwnerGameLayer->removeChild( m_pLivesLabel		);
	m_pglOwnerGameLayer->removeChild( m_pHighScoreLabel );
	m_pglOwnerGameLayer->removeChild( m_pLevelName		);
}


void CHUD::UpdateScore(int score) const
{
	DrawElement( m_pScoreLabel, static_cast<char*>("SCORE: "), score, Vec2( (m_pointOrigin).x + 1400.0f, ((m_pointOrigin).y + (m_sizeVisible).height) - 92 ), 20 );
}

void CHUD::UpdateLives(int lives) const
{
	DrawElement( m_pLivesLabel, static_cast<char*>("LIVES: "), lives, Vec2( (m_pointOrigin).x + 800.f, ((m_pointOrigin).y + (m_sizeVisible).height) - 40 ), 20 );
}

void CHUD::UpdateHighScore( int highScore ) const
{
	DrawElement( m_pHighScoreLabel, static_cast<char*>("HIGHSCORE: "), highScore, Vec2( (m_pointOrigin).x + 1680.f, ((m_pointOrigin).y + (m_sizeVisible).height) - 92 ) , 20 );
}

void CHUD::DrawElement( cocos2d::Label* label, char* labelText, int labelValue, cocos2d::Vec2 labelPos, int iFontSize, bool isLevelLabel) const
{
	if (m_pglOwnerGameLayer != nullptr)
	{
		m_pglOwnerGameLayer->removeChild( label );

		if (!isLevelLabel)
		{
			label = Label::createWithTTF( labelText + std::to_string( labelValue ), "fonts/arial.ttf", iFontSize );
		}
		else
		{
			label = Label::createWithTTF( labelText, "fonts/arial.ttf", iFontSize );
		}
		
		label->setGlobalZOrder( 3.f );
		label->setPosition( labelPos );

		m_pglOwnerGameLayer->addChild( label, 2 );
	}
}
