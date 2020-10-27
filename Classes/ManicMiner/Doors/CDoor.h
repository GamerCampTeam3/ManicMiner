//////////////////
/// Bib
//////////////////
#ifndef _CDOOR_H_
#define _CDOOR_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

// Forward class declaration
class CManicLayer;

class CDoor
	: public CGCObjSpritePhysics
{
private:

	CManicLayer& m_cManicLayer;									// Reference to manic layer to call interact event
	CGCFactoryCreationParams& m_FactoryCreationParams;			// This will be set in the constructor and dictate it's texture
	cocos2d::Vec2 m_v2ResetPosition;							// The position of the door inside the level
	
public:
	CDoor( CManicLayer& cLayer, CGCFactoryCreationParams& CreationParams, cocos2d::Vec2 ResetPosition);

	// Overriden functions from ObjSpritePhysics
	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnReset()					override;

	// Interact Event
	// Called when player collided with the door
	void InteractEvent();
};
#endif // #ifndef _CDOOR_H_