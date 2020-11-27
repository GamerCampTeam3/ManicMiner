//////////////////
/// Bib
//////////////////
#ifndef _CHUD_H_
#define _CHUD_H_


#include "ui/CocosGUI.h"

//-------------------------------------------------------- FWD CLS DECLARATION --------------------------------------------------------//	
enum class ELifeUpdateType;
class CCollectiblesGroup;
class CGCObjSprite;
class CManicLayer;
class CPlayer;

namespace cocos2d
{
	class Label;
}
//-------------------------------------------------------------------------------------------------------------------------------------//	



// A simple enum class used to determine the type of text to draw.
enum class ELabelType
{
	Text = 0,
	Number,
	TextAndNumber
};



class CHUD
{
public:
	// Constructor that takes in a collectibles group and layer reference
	// which are used to update the HUD
	CHUD ( CManicLayer& cLayer, cocos2d::Point pOrigin, cocos2d::Size visibleSize );
	~CHUD();

	void UpdateScore		( int score		);															// Redraws the text with the int param
	void UpdateHighScore	( int highScore	);															// Redraws the text with the int param
	void UpdateLives		( ELifeUpdateType eLifeUpdateType, int iCurrentLife );						// Redraws the sprites depending on the Enum.
	void FlushText() const;																				// Removes all sprites/labels, called on level end.
	void Init				( std::string szLevelName, int life,  int iScore, int iHighscore );			// Called by every level, with the current values.
	
private:
	//--------------------- MEMBER VARS ------------------------------------------------------//
	//
	//--------------------- TEXT LABELS ------------------------------------------------------//	
	cocos2d::Label*		m_pScoreValueLabel;					// Score Value
	cocos2d::Label*		m_pScoreTextLabel;					// Score Text 
	cocos2d::Label*		m_pHighScoreValueLabel;				// HighScore Value
	cocos2d::Label*		m_pHighscoreTextLabel;				// HighScore Text
	cocos2d::Label*		m_pLevelName;						// Level name Text
	
	CManicLayer*		m_pglOwnerGameLayer;				// Pointer to the current CManicLayer

	cocos2d::Point		m_pointOrigin;				
	cocos2d::Size		m_sizeVisible;

	//--------------------- VECTOR 2s --------------------------------------------------------//	
	cocos2d::Vec2		m_v2ScoreTextPosition;				// Position of the Score Text
	cocos2d::Vec2		m_v2HighscoreTextPosition;			// Position of the HighScore Text
	cocos2d::Vec2		m_v2ScoreValuePosition;				// Position of the Score Value
	cocos2d::Vec2		m_v2HighscoreValuePosition;			// Position of the HighScore Value	
	cocos2d::Vec2		m_v2LevelNamePosition;				// Position of the Level name Text

	//--------------------- SPRITES RELATED ---------------------------------------------------//	
	static const int	m_kiMaximumLives = 10;				// Maximum amount of sprites to draw	
	CGCObjSprite*		m_apcLives [m_kiMaximumLives ] ;	// Array of Sprites to represent lives
	float				m_fXPlacement;						// The X position of the first Sprite
	const float			m_kfYPlacement		= 1050.f;		// The Y position of the sprites
	const float			m_kfOffsetIncrement = 45.0f;		// The X offset between sprites

	//--------------------- FUNCTIONS----------------------------------------------------------//
	
	/// <summary>
	/// This updates the label with the given data.
	/// </summary>
	/// <param name="label"> <c> The label that should be changed. </param>
	/// <param name="labelText"><c> The text the label will display. </param>
	/// <param name="labelValue"><c> ... or the value. </param>
	/// <param name="eType"><c> An enum to allows us to draw just text, or a number, or both. </param>
	void  UpdateLabel		( cocos2d::Label* label, char* labelText, int labelValue, ELabelType eType ) const;

	/// <summary>
	/// This redraws the sprite with a new .plist.
	/// </summary>
	/// <param name="pSprite"> <c> The sprite that should be changed. </param>
	/// <param name="pzcPlist"><c> a char* that should point towards the .plist that it should be changed to. </param>
	void  ReDrawSprite		( CGCObjSprite* pSprite, const char* pzcPlist ) const;

	/// <summary>
	/// This redraws the sprite with a new .plist.
	/// </summary>
	/// <param name="label"> <c> The label that should be initialized. </param>
	/// <param name="color"><c> a cocos2d::Color4B that will be set to the label. </param>
	/// <param name="fontSize"> <c>   The size that the font will have. </param>
	/// <param name="textPos"> <c>  The position at which the label should be placed at. </param>
	/// <param name="alignment"> <c> And the alignment of the text using cocos2d::TextHAlignment enum. </param>
	void  InitLabel			( cocos2d::Label* label, cocos2d::Color4B color, float fontSize, cocos2d::Vec2 textPos, cocos2d::TextHAlignment alignment) const;


	//------------------ SPRITES LOCATION ------------------------------------------------------//
	// The Full Life Battery .plist
	const char* m_kpszPlistPlayerLifeFull = "TexturePacker/Sprites/Life/full_heart.plist";
	// The Lost Life Batter .plist
	const char* m_kpszPlistPlayerLifeLost = "TexturePacker/Sprites/Life/empty_heart.plist";

};

#endif // #ifndef _CHUD_H_