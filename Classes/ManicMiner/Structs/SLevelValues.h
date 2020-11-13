#ifndef _SLEVELVALUES_H_
#define _SLEVELVALUES_H_

#include "ManicMiner/Enums/ECollectibleRequirements.h"

// Anything that the level requires will go in here
// Simply add the data type and the initializer

struct SLevelValues
{
	// Constructor where the scenario is only collectibles required
	SLevelValues( ECollectibleRequirements eRequirements, int numOfCollectibles)
		: eCollectibleRequirements	( eRequirements )
		, iNumberOfSwitches			( 0 )
		, iNumberofCollectibles		( numOfCollectibles )
	{		
	}

	// Constructor where the scenario are both required
	SLevelValues( ECollectibleRequirements eRequirements, int numOfSwitches, int numOfCollectibles )
		: eCollectibleRequirements	( eRequirements )
		, iNumberOfSwitches			( numOfSwitches )
		, iNumberofCollectibles		( numOfCollectibles )
	{
	}

	SLevelValues()
		: eCollectibleRequirements(ECollectibleRequirements::Collectible)
		, iNumberOfSwitches (0)
		, iNumberofCollectibles(0)
	{		
	}

	

	ECollectibleRequirements	eCollectibleRequirements;
	int							iNumberOfSwitches;
	int							iNumberofCollectibles;
};

#endif //ifndef_SLEVELVALUES_H_