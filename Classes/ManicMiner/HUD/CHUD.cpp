#include <string>

#include "CHUD.h"

#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "ManicMiner/Layers/CManicLayer.h"

#include "platform/CCPlatformMacros.h"

#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "ManicMiner/Enums/ELifeUpdateType.h"


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
	, m_fXPlacement				( 700.f			)
{

	// We initialized them to 0 to keep the list clean, so we set them up here
	m_v2ScoreTextPosition		= cocos2d::Vec2( (m_pointOrigin).x + 1400.0f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 ); //	Score text
	m_v2HighscoreTextPosition	= cocos2d::Vec2( (m_pointOrigin).x + 1640.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 ); //	Highscore text
	
	m_v2ScoreValuePosition		= cocos2d::Vec2( (m_pointOrigin).x + 1500.0f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 ); //	Score value
	m_v2HighscoreValuePosition	= cocos2d::Vec2( (m_pointOrigin).x + 1785.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 92 ); //	Highscore value
	
	m_v2LevelNamePosition		= cocos2d::Vec2( (m_pointOrigin).x + 1570.f,		((m_pointOrigin).y + (m_sizeVisible).height) - 40 );	//	Level name


	// We new the labels now, and set the content on Init()
	m_pScoreTextLabel		= new cocos2d::Label();
	m_pScoreValueLabel		= new cocos2d::Label();
	m_pHighscoreTextLabel	= new cocos2d::Label();
	m_pHighScoreValueLabel	= new cocos2d::Label();
	m_pLevelName			= new cocos2d::Label();

	// The color of the text, that will be used to set the initial label values.
	cocos2d::Color4B textColor = cocos2d::Color4B( 165, 227, 251, 255 );

	// Then we initialize with the following params:
	// Label:		The current label we are editing.
	// Color4B:		The color of the text.
	// Font Size:	The size of the font.
	// Vector 2:	The position of the text on the screen.
	// Alignment:	How is the text aligned.
	InitLabel( m_pScoreTextLabel,		textColor, 20.0f, m_v2ScoreTextPosition,			TextHAlignment::LEFT	);
	InitLabel( m_pScoreValueLabel,		textColor, 20.0f, m_v2ScoreValuePosition,		TextHAlignment::CENTER	);
	InitLabel( m_pHighscoreTextLabel,	textColor, 20.0f, m_v2HighscoreTextPosition,		TextHAlignment::LEFT	);
	InitLabel( m_pHighScoreValueLabel,	textColor, 20.0f, m_v2HighscoreValuePosition,	TextHAlignment::CENTER	);
	InitLabel( m_pLevelName,			textColor, 25.0f, m_v2LevelNamePosition,			TextHAlignment::CENTER  );
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
	
	for (int i = 0; i < m_kiMaximumLives;i++)
	{
		m_apcLives[i]->RemoveFromParent();
	}
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



	// Draw life sprites here, all set as empty
	for (int i = 0; i < m_kiMaximumLives; i++)
	{
		// Create a vector that we will use modified value (to offset the sprites) every loop.
		const cocos2d::Vec2 v2InitialPlacement = cocos2d::Vec2( m_fXPlacement, m_kfYPlacement );

		// We new and set the sprites here.
		m_apcLives[i] = new CGCObjSprite();
		m_apcLives[i]->CreateSprite( m_kpszPlistPlayerLifeLost );							// Initial sprite should be empty (as only 3 of them would be full.
		m_apcLives[i]->SetSpriteGlobalZOrder( 2.f );										// We set the z order for it to be above.
		m_apcLives[i]->SetSpriteScale( 1.f, 1.f );								// Half the scale as they are too large (since there are 10 instead of 3.
		m_apcLives[i]->SetResetPosition( v2InitialPlacement );								// Set the reset position to be our temporary vector 2.
		m_apcLives[i]->GetSprite()->setPosition( m_apcLives[i]->GetResetPosition() );	// Set the position to be it's reset position.
		m_apcLives[i]->SetParent( m_pglOwnerGameLayer );									// Finally add it to the parent layer.
				
		m_fXPlacement += m_kfOffsetIncrement;												// Increment our X, so they do not overlap each other.
	}

	////--------------------------------------- TEMP CODE ---------------------------------------////
	// A temporary int that copies the original X placement position							   //
	// It will be used to add 3 empty hearts at the location of the original heart				   //
	// This is made since the hearts are meant to have the outline behind them, but since the code //
	// for drawing hearts works by replacing the sprite in an array, this is a quick fix.		   //
	static float fXPlacement = 700.0f;															   //
																								   //
	// A for loop to create the 3 extra empty hearts.											   //
	for (int i = 0; i < 3; i++)																	   //
	{																							   //
		const cocos2d::Vec2 v2Pos = cocos2d::Vec2( fXPlacement, m_kfYPlacement );			   //
																								   //
		CGCObjSprite* pcSprite = new CGCObjSprite();											   //
		pcSprite->CreateSprite( m_kpszPlistPlayerLifeLost );									   //
		pcSprite->SetSpriteGlobalZOrder( 2.0f );												   //
		pcSprite->SetSpriteScale( 1.0f, 1.0f );										   //
		pcSprite->SetResetPosition( v2Pos );													   //
		pcSprite->GetSprite()->setPosition( pcSprite->GetResetPosition() );					   //
		pcSprite->SetParent( m_pglOwnerGameLayer );												   //
																								   //
		fXPlacement += m_kfOffsetIncrement;														   //
	}																							   //
																								   //
	// and finally we reset the static int since otherwise the next levels will offset again.	   //
	fXPlacement = 700.0f;																		   //
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Set full the current lives (this will change level to level)
	for (int i = 0; i < life; i++)
	{
		ReDrawSprite( m_apcLives[i], m_kpszPlistPlayerLifeFull );						    // Here we then set the sprites to show full depending on player lives.
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
			if (m_apcLives != nullptr)
			{
				ReDrawSprite( m_apcLives[i], m_kpszPlistPlayerLifeFull );
			}
			else
			{
				// Scenario if the player reaches more than 10 lives (highly unlikely without cheating).
			}
			break;

		case ELifeUpdateType::Minus:
			if (m_apcLives != nullptr)
			{
				ReDrawSprite( m_apcLives[i], m_kpszPlistPlayerLifeLost );
			}
			else
			{
				// Scenario if the player reaches more than 10 lives (highly unlikely without cheating).
			}
			break;
	}
}

void CHUD::UpdateHighScore( int highScore ) 
{
	UpdateLabel( m_pScoreValueLabel, nullptr, highScore, ELabelType::Number );
}

void CHUD::ReDrawSprite( CGCObjSprite* pSprite,  const char* pzcPlist) const
{
	if (pSprite != nullptr)
	{
		pSprite->RemoveFromParent();													   		// Remove *this from parent layer.
		pSprite->CreateSprite( pzcPlist );												   		// Set the sprite (texture)
		pSprite->SetSpriteGlobalZOrder( 2.f );													// Set the Z order.
		pSprite->GetSprite()->setPosition( pSprite->GetResetPosition() );				   	// Set it's position to it's original reset position.
		pSprite->SetSpriteScale( 1.0f, 1.0f );										// Set the scale.
		pSprite->SetParent( m_pglOwnerGameLayer );										   		// Finally add it to the parent layer.
	}
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
