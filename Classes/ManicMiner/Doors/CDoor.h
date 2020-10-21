#ifndef _CDOOR_H_
#define _CDOOR_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

class CManicLayer;

class CDoor
	: public CGCObjSpritePhysics
{
private:
	CManicLayer& m_cManicLayer;
	CGCFactoryCreationParams& m_FactoryCreationParams;
	cocos2d::Vec2 m_v2ResetPosition;

	
public:
	CDoor( CManicLayer& cLayer, CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition);


	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset() override;

	
	void InteractEvent();
};
#endif // #ifndef _CDOOR_H_