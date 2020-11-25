////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer November 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"
#include "GCObjEugene.h"
//#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/Layers/CMLEugenesLair.h"

#ifndef TINYXML2_INCLUDED
    #include "external\tinyxml2\tinyxml2.h"
#endif

#ifndef _GCLEVELLOADER_OGMO_H_
    #include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

USING_NS_CC;

GCFACTORY_IMPLEMENT_CREATEABLECLASS( CGCObjEugene );

class  CMLEugenesLair;

CGCObjEugene::CGCObjEugene()
	: CGCObjEnemy(GetGCTypeIDOf(CGCObjEugene)) 
{
	m_bAngryEugeneTriggered = false;
}

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjEugene::~CGCObjEugene()
{
	
}


//////////////////////////////////////////////////////////////////////////
//Function to provide the frame update of this object
//////////////////////////////////////////////////////////////////////////
//virtual function
void CGCObjEugene::VOnUpdate(float fTimeStep)
{
	// Call base class version first.
	CGCObjEnemy::VOnUpdate(fTimeStep);
	
	// Do Eugene specific functionality below:
	
	// ie, if accessor 'set euguene flashing'
	// animation?

	if (m_bAngryEugeneTriggered)
	{
		SetSpriteRotation(fTimeStep);





	}




}


void  CGCObjEugene::VOnResourceAcquire(void)
{
	CGCObjEnemy::VOnResourceAcquire();

	SetName("Eugene");


}


void CGCObjEugene::TriggerEugenesAlternativeAnimation(void)
{
	m_bAngryEugeneTriggered = true;
	

}

