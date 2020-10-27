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
	: public CGCObject
{
public:
	// Constructor that takes in a collectibles group and layer reference
	// which are used to update the HUD
	CHUD(CPlayer& cPlayer, CManicLayer& cLayer, CCollectiblesGroup& collectiblesGroup);
	
private:
	// Member Variables
	cocos2d::Label*		m_pLivesLabel;			// Lives text
	cocos2d::Label*		m_pScoreLabel;			// Score text
	CPlayer*			m_pcPlayer;				// Player pointer
	CManicLayer*		m_pglOwnerGameLayer;	// manic layer pointer
	CCollectiblesGroup*	m_pCollectiblesGroup;	// Collectibles group pointer

	// Object Overrides
	void VOnUpdate( float fTimeStep ) override;
	void VOnReset() override;

	// Updates the text
	void UpdateLabel();
};

#endif // #ifndef _CHUD_H_