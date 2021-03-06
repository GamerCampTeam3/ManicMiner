#ifndef _CGAMEOVERSCENE_H_
#define _CGAMEOVERSCENE_H_

#include "cocos2d.h"

//////////////////////////////////////////////////////////////////////////
// class deriving from CCLayer that creates our main menu scene
//////////////////////////////////////////////////////////////////////////
class CLevelManager;

class CGameOverScene
	: public cocos2d::CCLayer
{
public:
	// cocos2d-x classes all have a static create() function
	// This macro implements the default behaviour of create()
	// i.e. news an instance, calls init, and calls autorelease 
	CREATE_FUNC( CGameOverScene );

	// Explicit constructor - called from the CREATE_FUNC() macro
	// n.b. this must back-chain calls to the base class versions of init()
	// or the behaviour of your code will be 'unexpected'
	virtual bool init();

	// callback passed to the CCMenu system to be called when
	// the Game Start button is pushed
	void CB_OnGameStartButton( Ref* pSender );

	void CB_OnGameExitButton( Ref* pSender );

	// The cocos2d convention is to have the root layer of a scene
	// be able to create and return a CCScene* (set to autorelease)
	// which has an instance of itself as a child
	static cocos2d::Scene* scene( CLevelManager& rcLevelManager );

	void SetLevelManager( CLevelManager& rcLevelManager );
private:

	// Pointer to CLevelManager that creates this instance
	CLevelManager* m_pcLevelManager;
	std::string GetPhrase( unsigned int score ) const;
};
#endif