// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
//	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	DEPRECATED	!	//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:	This b2RayCastCallback is my first attempt at getting the Solar Light mechanic to work;					//
//				Essentially this is just a copy of the previously created up-and-running CPlayerB2RayCastCallBack		//
//				with minor modifications so it would check for enemies and platforms instead of bricks.					//
//																														//
//				After some thought, I concluded an AABB Query would better fit this idea, so this class was abandoned.	//
//																														//
// See Also	:	CSolarLightB2AABBQueryCB																				//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _SOLAR_LIGHT_B2RAYCAST_CALLBACK_H
#define _SOLAR_LIGHT_B2RAYCAST_CALLBACK_H

#include "Box2D/Dynamics/b2WorldCallbacks.h"

enum class ESolarLightRayCastHitResult
{
	None,
	Enemy,
	Platform
};

class CSolarLightB2RayCastCallBack : public b2RayCastCallback
{
private:
	// Hit result
	ESolarLightRayCastHitResult m_eHitResult;

public:
	// Special Report Fixture, checks if fixture is enemy / platform
	virtual float32 ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction ) override;

	void ResetFlag();
	ESolarLightRayCastHitResult GetRaycastHitResult() const;
};
#endif // End of #ifndef _SOLAR_LIGHT_B2RAYCAST_CALLBACK_H