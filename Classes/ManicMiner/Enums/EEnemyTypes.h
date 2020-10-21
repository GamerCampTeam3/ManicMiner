#ifndef _EENEMY_
#define _EENEMY_

////////////////////////////////////////////////////////////////////////////////////////////////////

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
		EMovementAxis_UpDown, 
		EMovementAxis_LeftRight 
	};


};
#endif //ifndef _EENEMY_		#pragma once
