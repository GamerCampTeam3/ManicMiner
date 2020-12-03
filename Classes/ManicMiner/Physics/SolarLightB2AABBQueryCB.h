#ifndef _SOLAR_LIGHT_B2_AABB_QUERY_CB_H_
#define _SOLAR_LIGHT_B2_AABB_QUERY_CB_H_

#include "Box2D/Dynamics/b2WorldCallbacks.h"

enum class ESolarLightQueryResult
{
	None,
	Enemy,
	Platform,
	Player
};


class CSolarLightB2AABBQueryCB : public b2QueryCallback
{
public:
	virtual bool ReportFixture( b2Fixture* fixture ) override;
	void Reset();
	ESolarLightQueryResult GetQueryResult() const;
private:
	ESolarLightQueryResult m_eQueryResult;	// Defaults to None
};
#endif // End of #ifndef _SOLAR_LIGHT_B2_AABB_QUERY_CB_H_