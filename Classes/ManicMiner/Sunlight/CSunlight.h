////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CSUNLIGHT_H_
#define _CSUNLIGHT_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

enum class EEffect
{
	DrainAir
};

class CAirManager;

class CSunlight : public CGCObjSpritePhysics
{
public:

	CSunlight(CAirManager* pcAirManager);

	GCFACTORY_DECLARE_CREATABLECLASS(CSunlight);

	void VOnResourceAcquire() override;

	void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;

	void VOnResourceRelease() override;

	void VOnReset() override;

private:

	// holds a pointer to the AirManager class in the level - might no need this, because all collision based is handled in maniclayer
	CAirManager* m_pcAirManager;

	EEffect m_eEffect;

	b2Body* m_pcSpriteBody;
};

#endif