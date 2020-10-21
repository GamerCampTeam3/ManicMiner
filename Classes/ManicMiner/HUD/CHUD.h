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
	CHUD(CPlayer& cPlayer, CManicLayer& cLayer, CCollectiblesGroup& collectiblesGroup);
	~CHUD();

	void UpdateLabel();
	//void init(CManicLayer& cLayer);
	
private:
	cocos2d::Label* m_pLivesLabel;
	cocos2d::Label* m_pScoreLabel;
	CPlayer* m_pcPlayer;
	CManicLayer* m_pglOwnerGameLayer;
	CCollectiblesGroup* m_pCollectiblesGroup;
	
	void VOnUpdate( float fTimeStep ) override;
	void VOnReset() override;
};

#endif // #ifndef _CHUD_H_