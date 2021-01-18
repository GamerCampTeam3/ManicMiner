// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:	This b2RayCastCallback is a RayCast used for the player jump. When trying to jump, we first use this	//
//				to make sure no brick platform is directly above the player's head. Back in Module 1, if the player		//
//				jumped in this condition, it would cause a bug and the player would get stuck forever.					//
//				I implemented this to prevent said bug, and later during Module 2 after changing a lot of the collision	//
//				logic, I found out this was no longer needed because the bug was no longer happening.					//
//				This RayCast check is still being used though.															//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _PLAYER_B2RAYCAST_CALLBACK_H
#define _PLAYER_B2RAYCAST_CALLBACK_H

#include "Box2D/Dynamics/b2WorldCallbacks.h"

class CPlayerB2RayCastCallBack: public b2RayCastCallback
{
private:
	// RayCast Hit Brick Flag
	bool m_bDidRayHit;

public:

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ReportFixture																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Called for each subsequent fixture hit, this function is supposed to be inherited from				//
//					b2RayCastCallback, as its parent version is purely virtual.											//
//					The value that is returned (type float32) affects the continuation of the RayCast Query.			//
//					Extensive explanation in b2WorldCallbacks.h -> b2RayCastCallback::ReportFixture()					//
//																														//
//					This version of ReportFixture will check if the hit fixture is a brick platform, and if so,			//
//					set our relevant flag to true.																		//
//																														//
// Parameters	:	b2Fixture* fixture 																					//
//					fixture the fixture hit by the ray																	//
//																														//
//					const b2Vec2& point																					//
//					point the point of initial intersection																//
//																														//
//					const b2Vec2& normal																				//
//					normal the normal vector at the point of intersection												//
//																														//
// Returns		:	float32																								//
//																														//
// See Also		:	b2RayCastCallback::ReportFixture()																	//
// -------------------------------------------------------------------------------------------------------------------- //
	virtual float32 ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction ) override;

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ResetFlag																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Sets m_bDidRayHit flag to false, called right before testing the raycast							//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void ResetFlag();

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	GetDidRayHit																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	m_bDidRayHit Getter																					//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	bool GetDidRayHit() const;
};

#endif // End of #ifndef _PLAYER_B2RAYCAST_CALLBACK_H