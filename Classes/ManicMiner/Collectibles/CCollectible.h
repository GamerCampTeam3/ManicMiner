#ifndef _CCOLLECTIBLE_H_
#define _CCOLLECTIBLE_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#endif


#include "I_Interactible.h"




//////////////////////////////////////////////////////////////////////////
// This is a sample class derived from CGCObject.
// 
// It demonstrates how you might do projectiles, it relies on 
// CGCObjGroupProjectilePlayer to manage the lifetimes of these.
//
//////////////////////////////////////////////////////////////////////////
class CCollectible
	: public CGCObjSpritePhysics, public I_Interactible
{
private:
	int m_iIndex;

public:
	CCollectible();
	CCollectible(cocos2d::Vec2 initialPos);


	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CCollectible()
	{}



	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface

	virtual void VOnResourceAcquire() override;

	virtual void VOnReset() override;

	virtual void VOnUpdate(f32 fTimeStep) override;

    void InteractEvent() override;

	//Getters and Setters
	int GetIndex() { return m_iIndex; };
	void SetIndex(int index) { m_iIndex = index; };
	
};





#endif // #ifndef _CCOLLECTIBLE_H_