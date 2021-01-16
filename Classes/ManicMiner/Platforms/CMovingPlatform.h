////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Umeer Rama
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CMOVINGPLATFORM_H_
#define _CMOVINGPLATFORM_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "CPlatform.h"

// Forward declare direction enum
enum class EPlayerDirection;

class CMovingPlatform : public CPlatform
{
public:
	CMovingPlatform();
	//CMovingPlatform( CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition, const EPlayerDirection eDirectionLock );
	void InitDirectionalLock(const EPlayerDirection eDirectionLock);

	GCFACTORY_DECLARE_CREATABLECLASS(CMovingPlatform);
	
	void VOnResourceAcquire() override;

	void VHandleFactoryParams(const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition) override;
	
	void VOnReset() override;


	// DirectionLock Getter
	const EPlayerDirection GetDirectionLock();
private:
	// the animation is not being used currently
	cocos2d::Animation* m_pcMovingAnim;
	EPlayerDirection m_eDirectionLock;
};
#endif