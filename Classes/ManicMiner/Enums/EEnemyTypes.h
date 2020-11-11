#ifndef _EENEMY_
#define _EENEMY_

////////////////////////////////////////////////////////////////////////////////////////////////////
// This class defines the globally accessible types associated with the Enemy classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
class EnemyTypes
{
public:

	enum class EEnemyId {
		EDuck,
		EPenguin,
		EOstrich,
		EEugene,
		EKong,
		ELander    // (Skylab landing bay - destroys on impact)
	};
	
	enum class EMovementAxis {
		EMovementAxis_Vertical, 
		EMovementAxis_Horizontal 
	};

};
#endif //ifndef _EENEMY_		#pragma once
