
#ifndef _AIRMANAGER_H_
#define _AIRMANAGER_H_

#include "base/CCDirector.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "ui/CocosGUI.h"

class CGameManager;
class CManicLayer;
class CGCObjSprite;

namespace cocos2d {
	class Label;
}

enum class EAirState
{
	OutOfAir,
	HasAirLeft
};

enum class EAirDrainedState
{
	AirDrained,
	LevelCompleted
};

class CAirManager : public CGCObject
{
public:
	CAirManager(cocos2d::Point pOrigin, cocos2d::Size visibleSize );

	//CAirManager(class IGCGameLayer* pglOwnerGameLayer, cocos2d::Size* pv2VisibleSize, cocos2d::Point* ppOrigin);	// overloaded constructor
	virtual ~CAirManager();

	void LeavingLevel( CManicLayer& rNewManicLayer );

	void VOnUpdate(float fTimeStep) override;

	void VOnReset() override;

	void Init(  CManicLayer& rglOwnerGameLayer );
	
	bool GetHasInitialized();

	// Drain Functionality to drain out the remaining amount of air quicker to
	// calculate the amount of extra points the player will get based on the
	// amount of air left, when the player completes the level.
	void DrainAir();

	// Henri's Implementation of DrainAir for the Sunlight - needs to be reset back to default once the player doesn't touch the Sunlight
	void SunlightDrainAir(bool bShouldDrainAir);
	
private:

	EAirState m_eAirState;

	EAirDrainedState m_eAirDrainedState;
	
	class CManicLayer* m_pglOwnerGameLayer;

	class cocos2d::Director* m_pdDirector;
	
	///////////////////////////////////////////
	/// Visual Elements

	// Text Based Visual Representation of the Amount of Air left (in Percent)
	cocos2d::Label* m_plAirLabel;
	
	// Vignette On Screen Effect
	CGCObjSprite* m_pcGCSprAirVignette;

	// Visual Representation of the Progress Bar for Air
	cocos2d::ui::LoadingBar* m_pcLBAirBar;

	// Visual Representation of the Background Element for the Progress Bar for Air
	CGCObjSprite* m_pcGCSprAirBar;

	
	///////////////////////////////////////////
	/// Calculation variables

	// the max amount of air the player has - used for initializing RemainingAirAmount when game starts
	float m_fMaxAir;

	// the amount of air the player currently has
	float m_fRemainingAirAmount;

	// the amount of air the player has consumed - level based, not overall
	float m_fConsumedAirAmount;

	// the amount of air the player loses every frame
	float m_fReduceAirByAmountPerFrame;

	// the amount of air the player has left in percent
	float m_fRemainingAirPercentage;
	int	  m_iRemainingAirPercentage;

	// the amount of air the player has consumed in percent
	int m_iConsumedAirPercentage;

	// screen position of the Air Label
	cocos2d::Vec2 m_v2AirLabelPos;

	// Has Init run?
	bool m_bInitialized;

	cocos2d::Point m_pOrigin;
	cocos2d::Size m_visibleSize;

	float m_fDrainAirMultiplier;

	// Bib Edit:
	bool m_bDrainComplete;
	CGameManager* m_pcGameManager;

private:
	
	// updates air (timer) per frame and returns a bool stating whether there is any air left or not
	// @returnparam = true - has remaining air left
	// @returnparam = false - has no remaining air left
	bool UpdateAirTimer();

	void UpdateAirUIElements();
	
public:
	
	inline float fGetMaxAir()						  { return m_fMaxAir; }
	inline float fGetRemainingAirAmount()			  { return m_fRemainingAirAmount; }
	inline float fGetConsumedAirAmount()			  { return m_fConsumedAirAmount; }
	inline float fGetReduceAirByAmountPerFrame() 	  { return m_fReduceAirByAmountPerFrame; }
	inline float fGetFrameRate()					  { return m_pdDirector->getFrameRate(); }
	inline float fGetRemainingAirPercentage()		  { return m_fRemainingAirPercentage; }
	inline int	 iGetConsumedAirPercentage()		  { return m_iConsumedAirPercentage; }
	inline EAirState  eGetAirState()                  { return m_eAirState; }
	inline bool	 GetDrainComplete()					  { return m_bDrainComplete; }
	
	inline void eSetAirDrainedState(EAirDrainedState eAirDrainedState)		{ m_eAirDrainedState = eAirDrainedState; }
	inline EAirDrainedState eGetAirDrainedState()							{ m_eAirDrainedState; }
	void SetGameManager ( CGameManager* pcGameManager );
	
};
#endif

