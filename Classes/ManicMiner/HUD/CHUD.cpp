#include <string>

#include "CHUD.h"

#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "ManicMiner/Layers/CManicLayer.h"

#include "platform/CCPlatformMacros.h"

#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "ManicMiner/Enums/ELifeUpdateType.h"

#include "ManicMiner/Helpers/Helpers.h"


USING_NS_CC;

CHUD::CHUD( CManicLayer& cLayer, cocos2d::Point pOrigin, cocos2d::Size visibleSize )
	: m_pScoreValueLabel		( nullptr		)
	, m_pScoreTextLabel			( nullptr		)
	, m_pHighScoreValueLabel	( nullptr		)
	, m_pHighscoreTextLabel		( nullptr		)
	, m_pLevelName				( nullptr		)
	, m_pglOwnerGameLayer		( &cLayer		)
	, m_pointOrigin				( pOrigin		)
	, m_sizeVisible				( visibleSize	)
	, m_v2ScoreTextPosition		(0,0		)
	, m_v2HighscoreTextPosition	(0,0		)
	, m_v2ScoreValuePosition	(0,0		)
	, m_v2HighscoreValuePosition(0,0		)
	, m_v2LevelNamePosition		(0,0		)
	, m_fXPlacement				( 220.0f		)
{

	// We initialized them to 0 to keep the list clean, so we set them up here
	m_v2ScoreTextPosition		= cocos2d::Vec2( (m_pointOrigin).x + 1300.0f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 ); //	Score text
	m_v2HighscoreTextPosition	= cocos2d::Vec2( (m_pointOrigin).x + 1640.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 ); //	Highscore text
	
	m_v2ScoreValuePosition		= cocos2d::Vec2( (m_pointOrigin).x + 1390.0f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 ); //	Score value
	m_v2HighscoreValuePosition	= cocos2d::Vec2( (m_pointOrigin).x + 1785.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 ); //	Highscore value
	
	m_v2LevelNamePosition		= cocos2d::Vec2( (m_pointOrigin).x + 1510.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 40 );	//	Level name


	// We new the labels now, and set the content on Init()
	m_pScoreTextLabel		= new cocos2d::Label();
	m_pScoreValueLabel		= new cocos2d::Label();
	m_pHighscoreTextLabel	= new cocos2d::Label();
	m_pHighScoreValueLabel	= new cocos2d::Label();
	m_pLevelName			= new cocos2d::Label();

	// The color of the text, that will be used to set the initial label values.
	// White
	 cocos2d::Color4B const c4bTextColor = cocos2d::Color4B::WHITE;

	// Then we initialize with the following params:
	// Label:	The current label we are editing.
	// Color4B:	The color of the text.
	// Font Size:	The size of the font.
	// Vector 2:	The position of the text on the screen.
	// Alignment:	How is the text aligned.
	InitLabel( m_pScoreTextLabel,		c4bTextColor, 20.0f, m_v2ScoreTextPosition,			TextHAlignment::LEFT	);
	InitLabel( m_pScoreValueLabel,		c4bTextColor, 20.0f, m_v2ScoreValuePosition,		TextHAlignment::CENTER	);
	InitLabel( m_pHighscoreTextLabel,	c4bTextColor, 20.0f, m_v2HighscoreTextPosition,		TextHAlignment::LEFT	);
	InitLabel( m_pHighScoreValueLabel,	c4bTextColor, 20.0f, m_v2HighscoreValuePosition,	TextHAlignment::CENTER	);
	InitLabel( m_pLevelName,			c4bTextColor, 20.0f, m_v2LevelNamePosition,			TextHAlignment::CENTER  );
}

CHUD::~CHUD()
{
}

void CHUD::InitLabel( cocos2d::Label* label, cocos2d::Color4B color, float fontSize, cocos2d::Vec2 textPos, cocos2d::TextHAlignment alignment) const
{
	if (label != nullptr)
	{
		label->setTextColor( color );
		label->setTTFConfig( cocos2d::TTFConfig( "fonts/SMB2.ttf", fontSize ) );
		label->setGlobalZOrder( 3.f );
		label->setPosition( textPos );
		label->setAlignment( alignment );
		m_pglOwnerGameLayer->addChild( label, 2 );
	}
}


void CHUD::FlushText() const
{
	UpdateLabel( m_pLevelName, static_cast<char*>(" "), 0, ELabelType::Text );
}

void CHUD::Init(std::string szLevelName, int life, int iScore, int iHighscore)
{
	char* pczLevelname = _strdup( szLevelName.c_str() );

	// SCORE ---------------------------------------------------------------------------------------------------------------------------------------//
	// TEXT:
	UpdateLabel( m_pScoreTextLabel, static_cast<char*>("SCORE: "),0, ELabelType::Text );
	// VALUE: 
	UpdateLabel( m_pScoreValueLabel, nullptr, iScore, ELabelType::Number );
	// ---------------------------------------------------------------------------------------------------------------------------------------------//
	//
	//
	// HIGHSCORE -----------------------------------------------------------------------------------------------------------------------------------//
	// TEXT:
	UpdateLabel( m_pHighscoreTextLabel, "HIGHSCORE: ", 0, ELabelType::Text );
	// VALUE:
	UpdateLabel( m_pHighScoreValueLabel,  nullptr, iHighscore,  ELabelType::Number );
	// ---------------------------------------------------------------------------------------------------------------------------------------------//
	//
	//
	// LEVELNAME -----------------------------------------------------------------------------------------------------------------------------------//
	// TEXT:
	UpdateLabel( m_pLevelName,  pczLevelname, 0, ELabelType::Text );
	// ---------------------------------------------------------------------------------------------------------------------------------------------//


	// We draw lives here
	for (int i = 0; i < m_kiMaximumLives; i++)
	{
		// Create a vector that we will use modified value (to offset the sprites) every loop.
		const cocos2d::Vec2 v2InitialPlacement = cocos2d::Vec2( m_fXPlacement, m_kfYPlacement );

		// Draw the empty heart first then
		// draw a Full heart for every life the player currently has.
		// We only require to update the index when we create a Full heart.
		if (i < life)
		{
			DrawSprite( v2InitialPlacement, ELifeSpriteType::Empty);
			DrawSprite( v2InitialPlacement, ELifeSpriteType::Full,  i );
		}

		// Then finish drawing the rest of the empty hearts
		else
		{
			DrawSprite( v2InitialPlacement, ELifeSpriteType::Empty);
		}

		// Then we store the position of the current sprite being created inside our array.
		m_av2SpritePlacements[i] = v2InitialPlacement;

		// And finally we increment the X by the offset, so the next heart will be next to it.
		m_fXPlacement += m_kfOffsetIncrement;							// Increment our X, so they do not overlap each other.
	}
}


void CHUD::UpdateScore(int score) 
{
	UpdateLabel( m_pScoreValueLabel, nullptr, score, ELabelType::Number );
}

void CHUD::UpdateLives( ELifeUpdateType eLifeUpdateType, int iCurrentLife )
{
	int i = iCurrentLife;

	switch (eLifeUpdateType)
	{
		case ELifeUpdateType::Plus:
			i = --iCurrentLife;
			DrawSprite( m_av2SpritePlacements[i], ELifeSpriteType::Full, i );
			break;

		case ELifeUpdateType::Minus:
			if (m_apcLives[i] != nullptr)
			{
				m_apcLives[i]->RemoveFromParent();
			}
			break;
	}
}

void CHUD::UpdateHighScore( int highScore ) 
{
	UpdateLabel( m_pHighScoreValueLabel, nullptr, highScore, ELabelType::Number );
}

void CHUD::DrawSprite( const cocos2d::Vec2 kv2SpritePos, const ELifeSpriteType eLifeType, int index )
{
	const char* pszPath = nullptr;

	// We new the sprite we are going to work on now.
	CGCObjSprite* pcSprite = new CGCObjSprite();

	// Then depending on which plist to use, set the pszPath to that.
	// It is worth noting this wrapper function to easily create a sprite could be made in a way
	// that does not take in an index nor a Enum but instead a char* to the plist file location.
	// In this context, we know what we want, so it is set up in that way.
	switch (eLifeType)
	{
		case ELifeSpriteType::Empty:
			pszPath = m_kpszPlistPlayerLifeLost;
			break;
		
		case ELifeSpriteType::Full:
			pszPath = m_kpszPlistPlayerLifeFull;
			// Important! We have to set the array at the current index to point at the pcSprite we are working
			// This is absolutely required as it makes removing it from it's parent when you die extremely easy.
			m_apcLives[index] = pcSprite;
			break;
	}

	// Then we initialize the sprite with the values.
	// Again, since we know how we want the sprite, it is not required for those numbers to be parameters.
	pcSprite->CreateSprite( pszPath );
	pcSprite->SetSpriteGlobalZOrder( 2.f );
	pcSprite->SetSpriteScale( 1.f, 1.f );
	pcSprite->SetResetPosition( kv2SpritePos );
	pcSprite->GetSprite()->setPosition( pcSprite->GetResetPosition() );
	pcSprite->SetParent( m_pglOwnerGameLayer );
}


void CHUD::UpdateLabel( cocos2d::Label* label, char* labelText, int labelValue, ELabelType eType ) const
{
	// First we check if neither the Gamelayer nor the label we want to update is a nullptr
	if (m_pglOwnerGameLayer != nullptr && label != nullptr )
	{
		switch (eType)
		{
			case ELabelType::Text:
				label->setString( labelText );
				break;

			case ELabelType::Number:
				label->setString( std::to_string( labelValue ) );
				break;

			case ELabelType::TextAndNumber:
				label->setString( labelText + std::to_string( labelValue ) );
				break;
		}

		label->updateContent();
	}
}
