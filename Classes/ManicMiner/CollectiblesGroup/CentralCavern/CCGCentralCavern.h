//////////////////
/// Bib
//////////////////
#ifndef _CCGCENTRALCAVERN_H_
#define _CCGCENTRALCAVERN_H_

#include "GamerCamp/GCCocosInterface/SGCFactoryCreationParams.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"

// This class is a child of CCollectiblesGroup
class CCGCentralCavern
	: public CCollectiblesGroup
{
public:
	// Feeds the constructor values into parent constructor params
	 CCGCentralCavern( CManicLayer& cLayer, ECollectibleTypeRequired collectiblesTypeRequired, int numCollectiblesNeeded )
		 : CCollectiblesGroup( cLayer , collectiblesTypeRequired, numCollectiblesNeeded )
		 , m_cCreationParams ("Key", "TexturePacker/Sprites/Key/Key.plist", "Key", b2_staticBody, true)
	 {		 
	 }


private:
	 void GenerateCollectibles()	override;		// This creates the collectibles per level
	 void DeleteCollectibles();						// This will delete the objects, called in group release
	 void VOnGroupResourceRelease() override;		// in charge of releasing the resources

	 CGCFactoryCreationParams m_cCreationParams;	// Set in Constructor, this dictates the resource of the collectibles
};

#endif // #ifndef _CCGCENTRALCAVERN_H_