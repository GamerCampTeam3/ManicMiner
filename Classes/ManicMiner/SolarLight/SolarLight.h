// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:	This header file declares the CSolarLight class, our SolarLight manager.								//
//				As for responsibilities, this class will have its Update() called, in it, it will reconstruct the light	//
//				path once per frame, using AABB Querying,																//
//				and update the light's TexturedSpline representation accordingly.										//
//				If the light is overlaying with the player, the air bar drains much faster than normal.					//
//																														//
//				Present only on Level 19 "Solar Power Generator".														//
// -------------------------------------------------------------------------------------------------------------------- //
// See Also	:	Original Gameplay - https://youtu.be/BgUzteADsRI?t=1224													//
// -------------------------------------------------------------------------------------------------------------------- //
#ifndef _SOLAR_LIGHT_H
#define _SOLAR_LIGHT_H

// - Fwd Declares ----------------------------------------------------------------------------------------------------- //
class CAirManager;																										//
class TexturedSpline;																									//
class CMLSolarPowerGenerator;																							//
class CSolarLightB2RayCastCallBack;																						//
// -------------------------------------------------------------------------------------------------------------------- //

class CSolarLight
{
public:

// - Constructor ------------------------------------------------------------------------------------------------------ //
	CSolarLight( CMLSolarPowerGenerator& rcSolarLevel );
// - Deconstructor ---------------------------------------------------------------------------------------------------- //
	virtual ~CSolarLight();

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	Update																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Called once per frame inside the level's Update(),													//
//					this function calls both CheckCollisions() and BuildNewSpline() to generate this frame's spline		//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
	void Update();

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CheckCollisions																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Creates the TexturedSpline object, assigns initial properties such as initial path,					//
//					stroke size, texture, interpolation method.															//
//																														//
// Parameters	:	CMLSolarPowerGenerator& rcSolarLevel																//
//					Reference to the unique level that has the Solar Light mechanic,									//
//					we need it to add the TexturedSpline to this scene													//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	TexturedSpline																						//
// -------------------------------------------------------------------------------------------------------------------- //
	void Init( CMLSolarPowerGenerator& rcSolarLevel );

private:
// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CheckCollisions																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Performs all the calculations and loop logic that create a new m_vPath,								//
//					the path our light spline will follow. Called once per frame inside Update()						//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	b2World::QueryAABB(b2QueryCallback* callback, const b2AABB& aabb)									//
// -------------------------------------------------------------------------------------------------------------------- //
	void CheckCollisions();

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	BuildNewSpline																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose		:	Deletes the old spline representation, builds a new spline geometry using the updated m_vPath		//
//					Called once per frame inside Update(), just after CheckCollisions()									//
//																														//
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
//																														//
// See Also		:	TexturedSpline::buildGeometry()																		//
// -------------------------------------------------------------------------------------------------------------------- //
	void BuildNewSpline();


	// Reference to B2World to perform AABB Querying
	b2World& m_rcB2World;

	// Reference to Umeer's AirManager System, if the light overlaps with the player it makes the air drain faster
	CAirManager& m_rcAirManager;

	// Pointer to the TexturedSpline, the visual representation of our bouncy SolarLight
	TexturedSpline* m_pcTexturedSpline;
	
	// Scalar int that determines how smooth the curved spline will look, higher numbers are more expensive
	const int m_kiLightRoundness;

	// Vector of Points that will be used to create the light textured spline
	std::vector< cocos2d::Vec2 > m_vPath;

	// Coordinates where the light will be spawned
	const int m_kiStartX;
	const int m_kiStartY;
};
#endif //#ifndef _SOLAR_LIGHT_H