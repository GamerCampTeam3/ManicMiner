////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer - December 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJEUGENE_H_
#define _GCOBJEUGENE_H_

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif

#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

//////////////////////////////////////////////////////////////////////////
// 
//  This class defines an invidual instance of the Eugene character and is
//  a direct sub class of the Enemy class.
//
//  The following features required specifically for Eugene are provided:
//  - Ability to change the characters animation via a trigger mechanism
//    received from a public call (TriggerEugenesAlternativeAnimation).
//  - Ability to reset the animation to its 'Normal' state on level reset.
//
//  Design:
//  Eugene is defined as an OGMO entity - his name is specified in the OGMO parameter Name="Eugene".
//  This naming mechanism enables the CMLEugenesLair object to locate him in the list of instantiated game objects,
//  and then call his alternate TriggerEugenesAlternativeAnimation function via a pointer to him.
//
// Known limitations:
//  - None.
//////////////////////////////////////////////////////////////////////////

class CGCObjEugene
: public CGCObjEnemy
{
private:
			
public:

	CGCObjEugene();
		
	GCFACTORY_DECLARE_CREATABLECLASS(CGCObjEugene);

	virtual ~CGCObjEugene ();

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	void CGCObjEugene::VOnReset() override;

	//////////////////////////////////////////////////////////////////////////
	// specific public functions for this class
	void TriggerEugenesAlternativeAnimation(void);

};
#endif // #ifndef _GCOBJEUGENE_H_