//////////////////
/// Bib
//////////////////
/// DEPRECATED CODE

#ifndef _CCOLLECTIBLEOLD_H_
#define _CCOLLECTIBLEOLD_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#endif


#include "I_Interactible.h"


class CCollectibleManager;

class CCollectibleOLD
	: public CGCObjSpritePhysics, public I_Interactible
{
private:
	int m_iIndex;
	CCollectibleManager* m_rcCollectibleManager;

public:
	CCollectibleOLD();
	CCollectibleOLD(CCollectibleManager& collectibleManager);

	CCollectibleOLD(cocos2d::Vec2 initialPos);


	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CCollectibleOLD() {}

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface

	virtual void VOnResourceAcquire() override;

	virtual void VOnReset() override;

	virtual void VOnUpdate(f32 fTimeStep) override;
	//////////////////////////////////////////////////////////////////////////
	// overriden interface function
	void InteractEvent() override;


	void SetPosition(const b2Vec2 pos);

	//Getters and Setters
	int GetIndex() { return m_iIndex; };
	void SetIndex(int index) { m_iIndex = index; };

};





#endif // #ifndef _CCOLLECTIBLEOLD_H_