//////////////////
/// Bib
//////////////////
#ifndef _CHUD_H_
#define _CHUD_H_

#include "GamerCamp/GCObject/GCObject.h"

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
	CHUD ( CManicLayer& cLayer );
	~CHUD() { };

	void UpdateScore(int score);
	void UpdateHighScore(int highScore);
	void UpdateLives(int lives);
	void UpdateLevelName( const std::string szLevelName );
	
private:
	// Member Variables
	cocos2d::Label*		m_pLivesLabel;			// Lives text
	cocos2d::Label*		m_pScoreLabel;			// Score text
	cocos2d::Label*		m_pHighScoreLabel;		// HighScore text
	cocos2d::Label*		m_pLevelName;
	CManicLayer*		m_pglOwnerGameLayer;	// manic layer pointer

};

#endif // #ifndef _CHUD_H_