// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //

#include "ManicMiner/Layers/CMLSolarPowerGenerator.h"

#include "Box2D/Collision/b2Collision.h"

#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/Physics/SolarLightB2AABBQueryCB.h"
#include "SolarLight.h"
#include "ThirdParty/TexturedSpline.h"

#pragma region DebugLogDefines
//#define DEBUG_SOLAR_LIGHT
#pragma endregion DebugLogDefines

static const float s_kfPTM = 60.0f;

CSolarLight::CSolarLight( CMLSolarPowerGenerator& rcSolarLevel )
	: m_rcB2World( *rcSolarLevel.B2dGetWorld() )
	, m_rcAirManager ( rcSolarLevel.GetAirManager() )
	, m_pcTexturedSpline ( nullptr )
	, m_kiLightRoundness ( 3 )
	, m_kiStartX		( 23 )
	, m_kiStartY		( 16 )
{
	Init( rcSolarLevel );
}

CSolarLight::~CSolarLight()
{}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	Update																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CSolarLight::Update()
{
	// Find new light path
	CheckCollisions();
	// Generate new representation with new path
	BuildNewSpline();
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	Init																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CSolarLight::Init( CMLSolarPowerGenerator& rcSolarLevel )
{
	// Create the initial light path, which just goes straight down
	m_vPath =
	{
		{ ( m_kiStartX + 0.5f ) * s_kfPTM, m_kiStartY * s_kfPTM },
		{ ( m_kiStartX + 0.5f ) * s_kfPTM, m_kiStartY * s_kfPTM },
		{ ( m_kiStartX + 0.5f ) * s_kfPTM, s_kfPTM },
		{ ( m_kiStartX + 0.5f ) * s_kfPTM, s_kfPTM }
	};

	// Create the textured spline object
	if( auto pTexturedSpline = TexturedSpline::create( m_vPath, m_kiLightRoundness, "TexturePacker/Light/SolarLight.png" ) )
	{
		m_pcTexturedSpline = pTexturedSpline;

		// Set interpolation method
		m_pcTexturedSpline->m_interpolationType = TexturedSpline::InterpolationType::B_CUBIC;

		// Must be a power of 2 to wrap / repeat on the spline
		const float kfImageSize = 64.0f;

		// But we actually want it to be 60x60, not 64x64
		// So we set it to be scaled down to 60.0f / 64.0f
		const float kfSplineStrokeScale = s_kfPTM / kfImageSize;
		m_pcTexturedSpline->setStrokeScale( kfSplineStrokeScale );

		// Add spline to scene
		rcSolarLevel.addChild( m_pcTexturedSpline, -2 );
		
		// Reposition Spline
		const cocos2d::Vec2 v2TexturedSplineSize = m_pcTexturedSpline->getContentSize();
		m_pcTexturedSpline->setPosition( v2TexturedSplineSize * 0.5f );
	}
	else
	{
		CC_ASSERT( false );
	}
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	CheckCollisions																						//
// -------------------------------------------------------------------------------------------------------------------- //
void CSolarLight::CheckCollisions()
{
	// -----------------------------------------------------------------------------------------------------------------------------------------
	// We will be using AABB Querying to check the centre of each block in our light reflection sequence
	// If the centre of the light beam is touching or overlapping an enemy sensor, a reflection is triggered
	// If the block we are on is occupied by a platform, our path has come to an end
	//
	// Initially I had tried to implement this using raycasting, but then realised
	// it wouldn't work well, it was causing some issues and basically triggering twice when it should just be triggering once
	// and after examining the original gameplay clip a bit closer, I realised I only need to check a small box area inside each block
	// Original level gameplay clip: https://youtu.be/BgUzteADsRI?t=1224
	// Also read my diagram		'SolarLightExplanation.png'
	// We'll be checking the areas highlighted in red in my diagram
	// -----------------------------------------------------------------------------------------------------------------------------------------

	// To avoid bad querying regarding a certain block, we can't check the entire area, as it will detect every adjacent block
	
	//			I think this is because I decided, back in Module 1, that 1 Box2D unit would equal the length of 1 block...
	//			If I were to do it again I'd probably set it to be something like 10 or 100 Box2D units.

	// So we can avoid this physics engine issue by querying a slightly smaller area, 10x10 instead of 60x60 for example
	// Because Querying will be done with Box2D we have to use Box2D units
	// 1 block is 1 unit, so we can set the margin to be something like 0.40f, and we'll be querying 20% of the area inside the block
	// 0.40f is 40% of the block, so if we contract the block by 40% on both sides we contracted 80%, hence the 20% area we're left with
	const float kfInwardsMargin = 0.40f;
	
	// Prepare logic for while looping

	// Light only truly stops once it hits a platform
	bool bLightHitGround = false;

	// Light goes either down or left, so a bool is enough to keep track of direction
	bool bGoingDown = true;

	bool bHitPlayer = false;

	// We're starting from the very top of playable area, so on coordinate 16
	// And the light start shining down on block 23 across
	int iCurrentX = m_kiStartX;
	int iCurrentY = m_kiStartY;

	// Reset our path < std::vector < Vec2 > >
	m_vPath.clear();

	// Set 3 points on start of path, as the very minimum for our spline is 4 points
	// If no hits occur, light will impact the brick platform on the ground, adding the 4th and final point
	m_vPath.push_back( cocos2d::Vec2( 23.5f, 16.0f )* s_kfPTM );
	m_vPath.push_back( cocos2d::Vec2( 23.5f, 15.5f )* s_kfPTM );
	m_vPath.push_back( cocos2d::Vec2( 23.5f, 15.0f )* s_kfPTM );

	// Prepare physics components
	CSolarLightB2AABBQueryCB aabbQueryCB;
	b2AABB aabbQuery;
	
	int iNumOfReflections = 0;

	// Loop towards the next block, if it hasn't hit the ground yet
	while( !bLightHitGround )
	{
		// Readjust AABB Query area to be a small square inside our new current block
		aabbQuery.lowerBound = b2Vec2(	 iCurrentX + kfInwardsMargin,	  ( iCurrentY - 1 ) + kfInwardsMargin );
		aabbQuery.upperBound = b2Vec2( ( iCurrentX + 1 ) - kfInwardsMargin,	iCurrentY - kfInwardsMargin );

		// Reset existing flag on the callback
		aabbQueryCB.Reset();

		// Perform the AABB Query
		m_rcB2World.QueryAABB( &aabbQueryCB, aabbQuery );

		// Switch on Raycast Hit Result ( ENEMY / PLATFORM / PLAYER )
		if( aabbQueryCB.GetQueryResult() != ESolarLightQueryResult::None )
		{
			switch( aabbQueryCB.GetQueryResult() )
			{
			// -----------------------------------------------------------------------------------------------
			// ENEMY
			// -----------------------------------------------------------------------------------------------
			case ESolarLightQueryResult::Enemy:
			{
				// Everytime the light changes direction / bounces, we add 3 path points on the same block
				// These 3 points will ensure our path will be curved only inside this specific block,
				// it will look smooth because of our interpolation method. If we had less than 3 points,
				// the curve would be much bigger, it would look very inaccurate.
				// More than 3 points is unnecessary, the curve looks smooth enough, more points would only
				// make it more expensive.
				
				// If going down
				if( bGoingDown )
				{
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY ) * s_kfPTM );
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY - 0.5f ) * s_kfPTM );
					m_vPath.push_back( cocos2d::Vec2( iCurrentX, iCurrentY - 0.5f ) * s_kfPTM );
				}
				// If going left
				else
				{
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 1.0f, iCurrentY - 0.5f ) * s_kfPTM );
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY - 0.5f ) * s_kfPTM );
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY - 1.0f ) * s_kfPTM );
				}

				// Change light direction ( flip bool )
				bGoingDown = !bGoingDown;

#ifdef DEBUG_SOLAR_LIGHT
				CCLOG( "Hit Enemy on ( %d, %d )" , iCurrentX , iCurrentY );
#endif // DEBUG_SOLAR_LIGHT

				iNumOfReflections++;
			}
			break;
			// -----------------------------------------------------------------------------------------------
			// PLATFORM
			// -----------------------------------------------------------------------------------------------
			case ESolarLightQueryResult::Platform:
			{
			// If Raycast hit Platform, we're done with the Nvidia RTX this frame 
				bLightHitGround = true;

#ifdef DEBUG_SOLAR_LIGHT
				CCLOG( "Reached end of path on ( %d, %d )", iCurrentX, iCurrentY );
#endif //  DEBUG_SOLAR_LIGHT

				// If going down
				if (bGoingDown)
				{
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY ) * s_kfPTM );
				}
				// If going left
				else
				{
					m_vPath.push_back( cocos2d::Vec2( iCurrentX, iCurrentY - 0.5f ) * s_kfPTM );
				}
			}
			break;
			// -----------------------------------------------------------------------------------------------
			// PLAYER
			// -----------------------------------------------------------------------------------------------
			case ESolarLightQueryResult::Player:
			{
				bHitPlayer = true;
			}
			break;
			// We don't really need to do anything if the result is ESolarLightRayCastHitResult::None
			}
		}

		// Update the X and Y coordinates accordingly for the next iteration in this while loop
		if( bGoingDown )
		{
			iCurrentY--;
		}
		else
		{
			iCurrentX--;
		}
	}
#ifdef DEBUG_SOLAR_LIGHT
	CCLOG( "%d Reflections", iNumOfReflections );
#endif //  DEBUG_SOLAR_LIGHT

	// Update the air manager with whether the light is or not overlapping with the player this frame
	// Overlapping will make the air drain faster
	m_rcAirManager.SunlightDrainAir( bHitPlayer );
}

// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	BuildNewSpline																						//
// -------------------------------------------------------------------------------------------------------------------- //
void CSolarLight::BuildNewSpline()
{
	// Clean up the spline's saved path to set the new path
	// For some reason this is not handled inside its buildGeometry()
	m_pcTexturedSpline->path.clear();
	m_pcTexturedSpline->path = m_vPath;

	// Rebuild Spline Geometry
	m_pcTexturedSpline->buildGeometry( m_vPath, 3 );
	// Reposition Spline
	const cocos2d::Vec2 v2TexturedSplineSize = m_pcTexturedSpline->getContentSize();
	m_pcTexturedSpline->setPosition( v2TexturedSplineSize * 0.5f );
}