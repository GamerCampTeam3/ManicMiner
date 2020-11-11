#ifndef _SLEVELVALUES_H_
#define _SLEVELVALUES_H_


#include "ManicMiner/Enums/ECollectibleRequirements.h"

struct SLevelValues
{
	// Constructor where the scenario is only collectibles required
	SLevelValues( ECollectibleRequirements eRequirements, int numOfCollectibles)
		: collectibleRequirements	( eRequirements )
		, numSwitches				( 0 )
		, numCollectibles			( numOfCollectibles )
	{		
	}

	// Constructor where the scenario are both required
	SLevelValues( ECollectibleRequirements eRequirements, int numOfSwitches, int numOfCollectibles )
		: collectibleRequirements	( eRequirements )
		, numSwitches				( numOfSwitches )
		, numCollectibles			( numOfCollectibles )
	{
	}

	

	ECollectibleRequirements	collectibleRequirements;
	int							numSwitches;
	int							numCollectibles;
};

#endif //ifndef_SLEVELVALUES_H_