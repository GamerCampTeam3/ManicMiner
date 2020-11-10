#ifndef _PLAYER_B2RAYCAST_CALLBACK_H
#define _PLAYER_B2RAYCAST_CALLBACK_H

#include "Box2D/Dynamics/b2WorldCallbacks.h"

class CPlayerB2RayCastCallBack: public b2RayCastCallback
{
private:
	bool m_bDidRayHit;

public:
	virtual float32 ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction ) override;

	void ResetFlag();
	bool GetDidRayHit() const;
};

#endif // End of #ifndef _PLAYER_B2RAYCAST_CALLBACK_H