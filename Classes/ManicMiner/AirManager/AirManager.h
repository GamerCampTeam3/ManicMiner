
#ifndef _AIRMANAGER_H_
#define _AIRMANAGER_H_

#include "base/CCDirector.h"
#include "GamerCamp/GCObject/GCObject.h"
class CManicLayer;

namespace cocos2d {
	class Label;
}

enum EAirState
{
	EAS_OutOfAir,
	EAS_HasAirLeft
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

	void Init( class CMLCentralCavern& rglOwnerGameLayer );
	
	bool GetHasInitialized();

	void DrainAir();
	
private:

	EAirState m_eAirState;

	EAirDrainedState m_eAirDrainedState;
	
	class CMLCentralCavern* m_pglOwnerGameLayer;

	class cocos2d::Director* m_pdDirector;
	
	///////////////////////////////////////////
	/// Visual Elements

	// Visual Representation of the Progress Bar for Air
	class CGCObjSprite* m_pcGCSprAirBar;

	// Text Based Visual Representation of the Amount of Air left (in Percent)
	cocos2d::Label* m_plAirLabel;
	
	// Vignette On Screen Effect
	CGCObjSprite* m_pcGCSprAirVignette;
	
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
	int m_iRemainingAirPercentage;

	// the amount of air the player has consumed in percent
	int m_iConsumedAirPercentage;

	// screen position of the Air Label
	cocos2d::Vec2 m_v2AirLabelPos;

	// Has Init run?
	bool m_bInitialized;

	cocos2d::Point m_pOrigin;
	cocos2d::Size m_visibleSize;

	float m_fDrainAirMultiplier;

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
	inline int	 iGetRemainingAirPercentage()		  { return m_iRemainingAirPercentage; }
	inline int	 iGetConsumedAirPercentage()		  { return m_iConsumedAirPercentage; }
	inline EAirState  eGetAirState()                  { return m_eAirState; }
	
	inline void eSetAirDrainedState(EAirDrainedState eAirDrainedState)		{ m_eAirDrainedState = eAirDrainedState; }
	inline EAirDrainedState eGetAirDrainedState()							{ m_eAirDrainedState; }
	
};
#endif

