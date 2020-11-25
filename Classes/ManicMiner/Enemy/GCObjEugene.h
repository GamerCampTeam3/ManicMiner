////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer November 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJEUGENE_H_
#define _GCOBJEUGENE_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "ManicMiner/Enums/EEnemyTypes.h"




class  CMLEugenesLair;


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////

class CGCObjEugene
: public CGCObjEnemy
{
private:
			
public:

	CGCObjEugene();
		
	GCFACTORY_DECLARE_CREATABLECLASS(CGCObjEugene);
	

	 //static const CGCObjEugene* pEugene;
	bool m_bAngryEugeneTriggered;


	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjEugene();
	
	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	
	virtual void VOnUpdate			(float fTimeStep) override;
	virtual void VOnResourceAcquire	(void) override;

	void TriggerEugenesAlternativeAnimation(void);




};
#endif // #ifndef _GCOBJEUGENE_H_