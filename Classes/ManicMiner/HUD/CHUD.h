//////////////////
/// Bib
//////////////////
#ifndef _CHUD_H_
#define _CHUD_H_


#include "ui/CocosGUI.h"

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

	void UpdateScore(int score);
	void UpdateHighScore(int highScore);
	void UpdateLives(int lives);
	void UpdateLevelName( const std::string szLevelName );
	void FlushText();
	
private:
	// Member Variables
	cocos2d::Label*		m_pLivesLabel;			// Lives text
	cocos2d::Label*		m_pScoreLabel;			// Score text
	cocos2d::Label*		m_pHighScoreLabel;		// HighScore text
	cocos2d::Label*		m_pLevelName;
	CManicLayer*		m_pglOwnerGameLayer;	// manic layer pointer

	cocos2d::Point m_pOrigin;
	cocos2d::Size m_visibleSize;

	void  DrawElement(cocos2d::Label* label, char* labelText, int labelValue, cocos2d::Vec2 labelPos ) const;

};

#endif // #ifndef _CHUD_H_