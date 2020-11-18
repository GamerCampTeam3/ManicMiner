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

	void UpdateScore(int score) const;
	void UpdateHighScore(int highScore) const;
	void UpdateLives(int lives) const;
	void UpdateLevelName( const std::string szLevelName ) const;
	void FlushText() const;
	
private:
	// Member Variables
	cocos2d::Label*		m_pLivesLabel;			// Lives text
	cocos2d::Label*		m_pScoreLabel;			// Score text
	cocos2d::Label*		m_pHighScoreLabel;		// HighScore text
	cocos2d::Label*		m_pLevelName;
	CManicLayer*		m_pglOwnerGameLayer;	// manic layer pointer

	cocos2d::Point		m_pointOrigin;
	cocos2d::Size		m_sizeVisible;

	void  DrawElement(cocos2d::Label* label, char* labelText, int labelValue, cocos2d::Vec2 labelPos, int iFontSize, bool isLevelLabel = false ) const;
};

#endif // #ifndef _CHUD_H_