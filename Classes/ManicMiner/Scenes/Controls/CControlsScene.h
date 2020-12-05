//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018		+		Edited by everyone in the team								//
// Distributed under the MIT license - see readme.md														//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------------------------------------------- //
// See also	:	CLevelManager																						//
// ---------------------------------------------------------------------------------------------------------------- //

#ifndef __CCONTROLSSCENE_H__
#define __CCONTROLSSCENE_H__

#include "cocos2d.h"

//////////////////////////////////////////////////////////////////////////
// class deriving from CCLayer that creates our main menu scene
//////////////////////////////////////////////////////////////////////////
class CLevelManager;

class CControlsScene
	: public cocos2d::CCLayer
{
public:
	// cocos2d-x classes all have a static create() function
	// This macro implements the default behaviour of create()
	// i.e. news an instance, calls init, and calls autorelease 
	CREATE_FUNC( CControlsScene );

	// Explicit constructor - called from the CREATE_FUNC() macro
	// n.b. this must back-chain calls to the base class versions of init()
	// or the behaviour of your code will be 'unexpected'
	virtual bool init();

	// callback passed to the CCMenu system to be called when
	// the Game Start button is pushed
	void CB_OnReturnToMenu( Ref* pSender );


	void CB_OnFullScreenButton( Ref* pSender );

	// The cocos2d convention is to have the root layer of a scene
	// be able to create and return a CCScene* (set to autorelease)
	// which has an instance of itself as a child
	static cocos2d::Scene* scene( CLevelManager& rcLevelManager );

	// -------------------------------------------------------------------------------------------------------------------- //
	// Henrique edit																										//
	// -------------------------------------------------------------------------------------------------------------------- //
	// Function		:	SetLevelManager																						//
	// -------------------------------------------------------------------------------------------------------------------- //
	// Purpose		:	Sets m_pcLevelManager, so that we can proceed to the first level									//
	//					through CB_OnGameStartButton()																		//
	//																														//
	// Parameters	:	CLevelManager& rcLevelManager																		//
	//					reference to CLevelManager that is creating this CMenuLayer isntance								//
	//																														//
	// See also		:	CLevelManager, its constructor and CLevelManager::GoToMainMenu()									//
	// -------------------------------------------------------------------------------------------------------------------- //
	void SetLevelManager( CLevelManager& rcLevelManager );
private:

	// Pointer to CLevelManager that creates this instance
	CLevelManager* m_pcLevelManager;
	bool m_bFullScreenSwitch;
	cocos2d::Menu* m_pMenu;
	std::string m_strPath;

	void CreateFullScreenButton();
};

#endif // __CCONTROLSSCENE_H__