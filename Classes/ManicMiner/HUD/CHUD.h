//////////////////
/// Bib
//////////////////
#ifndef _CHUD_H_
#define _CHUD_H_


#include "ui/CocosGUI.h"

class CGCObjSprite;
class CCollectiblesGroup;
class CManicLayer;
class CPlayer;

namespace cocos2d
{
	class Label;
}

class CHUD
{
public:
	// Constructor that takes in a collectibles group and layer reference
	// which are used to update the HUD
	CHUD ( CManicLayer& cLayer, cocos2d::Point pOrigin, cocos2d::Size visibleSize );
	~CHUD() { };

	void UpdateScore(int score) ;
	void UpdateHighScore(int highScore) ;
	void UpdateLives(int lives) ;
	void UpdateLevelName( const std::string szLevelName ) ;
	void FlushText() const;

	void Init(std::string szLevelName, int iLives = 0, int iScore = 0, int iHighscore = 0);
	
private:
	// Member Variables
	cocos2d::Label*		m_pLivesLabel;			// Lives text
	cocos2d::Label*		m_pScoreLabel;			// Score text
	cocos2d::Label*		m_pScoreTextLabel;
	cocos2d::Label*		m_pHighScoreLabel;		// HighScore text
	cocos2d::Label*		m_pHighscoreTextLabel;
	cocos2d::Label*		m_pLevelName;

	CManicLayer*		m_pglOwnerGameLayer;	// manic layer pointer

	cocos2d::Point		m_pointOrigin;
	cocos2d::Size		m_sizeVisible;
	cocos2d::Vec2		m_v2LivesPosition;
	cocos2d::Vec2		m_v2ScorePosition;
	cocos2d::Vec2		m_v2HighscorePosition;
	cocos2d::Vec2		m_v2LevelNamePosition;

	float testX;

	
	void  DrawElement(cocos2d::Label* label, char* labelText, int labelValue, cocos2d::Vec2 labelPos, float fFontSize ) const;

	const char* m_kpszPlistPlayerLifeFull = "TexturePacker/Sprites/Life/cc_life_full.plist";
	const char* m_kpszPlistPlayerLifeLost = "TexturePacker/Sprites/Life/cc_life_lost.plist";
};

#endif // #ifndef _CHUD_H_