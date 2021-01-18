// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:	This is a custom b2QueryCallback, we use CSolarLightB2AABBQueryCB to calculate the Solar Light's path	//
//				inside level 19 - Solar Power Generator																	//
//																														//
// See Also	:	CSolarLight.h																							//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _SOLAR_LIGHT_B2_AABB_QUERY_CB_H_
#define _SOLAR_LIGHT_B2_AABB_QUERY_CB_H_

#include "Box2D/Dynamics/b2WorldCallbacks.h"
// -------------------------------------------------------------------------------------------------------------------- //
// Enum Class	:	ESolarLightQueryResult																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Used to evaluate Solar Light's query detected entity												//
//					Affects whether the Solar Light bounces, changing directions, or if the path has come to an end		//
// -------------------------------------------------------------------------------------------------------------------- //
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
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ReportFixture																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Called for each fixture found in the AABB query, this version of ReportFixture						//
//					Checks if the detected fixture is either an Enemy, Platform or Player, and sets						//
//					m_eQueryResult accordingly																			//
//																														//
//					The query is performed every frame for the Solar Power Generator Level ( Level 19 ), in which there //
//					is a unique Bouncy Light mechanic. This query is used to calculate the light's bouncy path			// 
//																														//
// Parameters	:	b2Fixture* fixture																					//
//					Encountered Fixture																					//
//																														//
// Returns		:	bool																								//
//					returning false will terminate the query															//
//																														//
// See Also		:	b2QueryCallback::ReportFixture() and CSolarLight::CheckCollisions()									//
// -------------------------------------------------------------------------------------------------------------------- //
	virtual bool ReportFixture( b2Fixture* fixture ) override;

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	Reset																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets the m_eQueryResult flag to None. Called before performing the query, once per block iteration.	//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void Reset();

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GetQueryResult																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	m_eQueryResult Getter																				//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	ESolarLightQueryResult																				//
// -------------------------------------------------------------------------------------------------------------------- //
	ESolarLightQueryResult GetQueryResult() const;

private:
// Type of the entity detected by the AABB Query on a specific block region
	ESolarLightQueryResult m_eQueryResult;
};
#endif // End of #ifndef _SOLAR_LIGHT_B2_AABB_QUERY_CB_H_