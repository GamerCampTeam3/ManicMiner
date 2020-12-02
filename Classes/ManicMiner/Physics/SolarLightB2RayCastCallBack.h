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
	ESolarLightRayCastHitResult m_eHitResult;

public:
	virtual float32 ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction ) override;

	void ResetFlag();
	ESolarLightRayCastHitResult GetRaycastHitResult() const;
};
#endif // End of #ifndef _SOLAR_LIGHT_B2RAYCAST_CALLBACK_H