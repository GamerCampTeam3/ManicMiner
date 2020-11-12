////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CCRUMBLINGPLATFORM_H_
#define _CCRUMBLINGPLATFORM_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "CPlatform.h"
#include "2d/CCActionInterval.h"

// 
enum class ECrumbleState
{
	Stage_0,
	Stage_1,
	Stage_2,
	Stage_3,
	Stage_Destroy
};

class CCrumblingPlatform : public CPlatform
{
public:
	CCrumblingPlatform();
	// overloaded constructor
	//CCrumblingPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition );
	GCFACTORY_DECLARE_CREATABLECLASS(CCrumblingPlatform);
	
	void VOnResourceAcquire() override;

	void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;
	
	void VOnUpdate(float fTimeStep) override;

	void VOnReset() override;

	void VOnResourceRelease() override;
	
	// Initiates Crumbling Timer and Animation
	void InitiateCrumbling();

	// Stops Crumbling Timer
	void StopCrumbling();
	
private:

	////////////////////////////////////////////////////////////////////////
	/// Crumbling Platforms

	ECrumbleState m_eCrumbleState;

	// whether the crumbling timer should start ticking - set by InitiateCrumbling and StopCrumbling
	bool m_bInitiatedCrumbling;

	// current amount of time left, until the crumblingplatform gets destroyed/disables physicsbody
	float m_fCurrentCrumblingTimer;
	// amount to reduce the timer by on every frame
	float m_fReduceCrumblingTimerBy;

	class cocos2d::Director* m_pcDirector;

	//cocos2d::Map<char*, cocos2d::Animation*> m_mAnimationMap;
	char* m_pszAnimations[6];
	cocos2d::Animation* m_pcAnimations[6];

	// Loads all animations that are required for the Crumbling Effect of this Platform, when the player steps onto it 
	void LoadAnimations();

	// plays crumbling animation based on the newly passed in ECrumbleState, only if the new ECrumbleState is different
	// to the current ECrumbleState the Platform is in 
	void UpdateCrumblingPlatform(ECrumbleState eNewCrumbleState);

	/// Crumbling Platforms
	////////////////////////////////////////////////////////////////////////

	bool IsInRange(float NumToCompare, float MinRange, float MaxRange);
	
};

#endif

