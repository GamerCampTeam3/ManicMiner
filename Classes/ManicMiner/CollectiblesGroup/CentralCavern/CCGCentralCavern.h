#ifndef _CCGCENTRALCAVERN_H_
#define _CCGCENTRALCAVERN_H_

#include "GamerCamp/GCCocosInterface/SGCFactoryCreationParams.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"

class CCGCentralCavern
	: public CCollectiblesGroup
{
public:
	 CCGCentralCavern(CManicLayer& cLayer, ECollectibleTypeRequired collectiblesTypeRequired, int numCollectiblesNeeded)
		 : CCollectiblesGroup( cLayer , collectiblesTypeRequired, numCollectiblesNeeded)
		 , m_cCreationParams ("Key", "TexturePacker/Sprites/Key/Key.plist", "Key", b2_staticBody, true)
	 {
		 
	 }

	 CGCFactoryCreationParams m_cCreationParams;
	

private:
	 void GenerateCollectibles() override;
	 void DeleteCollectibles();
	 void VOnGroupResourceRelease() override;
	
};





#endif // #ifndef _CCGCENTRALCAVERN_H_