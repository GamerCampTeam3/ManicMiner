#include "ManicMiner/Layers/CMLSolarPowerGenerator.h"

#include "Box2D/Collision/b2Collision.h"

#include "ManicMiner/Physics/SolarLightB2AABBQueryCB.h"
#include "SolarLight.h"
#include "ThirdParty/TexturedSpline.h"

static const float s_kfPTM = 60.0f;

CSolarLight::CSolarLight( CMLSolarPowerGenerator& rcSolarLevel )
	: m_rcB2World( *rcSolarLevel.B2dGetWorld() )
	, m_pcTexturedSpline ( nullptr )
	, m_kiLightRoundness ( 3 )
	, m_kiStartX		( 23 )
	, m_kiStartY		( 16 )
{
	Init( rcSolarLevel );
}

CSolarLight::~CSolarLight()
{
}

void CSolarLight::Update()
{
	CheckCollisions();
	BuildNewSpline();
}

void CSolarLight::Init( CMLSolarPowerGenerator& rcSolarLevel )
{
	m_vPath =
	{
		{ ( m_kiStartX + 0.5f ) * s_kfPTM, m_kiStartY * s_kfPTM },
		{ ( m_kiStartX + 0.5f ) * s_kfPTM, m_kiStartY * s_kfPTM },
		{ ( m_kiStartX + 0.5f ) * s_kfPTM, s_kfPTM },
		{ ( m_kiStartX + 0.5f ) * s_kfPTM, s_kfPTM }
	};

	if( auto pTexturedSpline = TexturedSpline::create( m_vPath, m_kiLightRoundness, "TexturePacker/Light/SolarLight.png" ) )
	{
		m_pcTexturedSpline = pTexturedSpline;
		m_pcTexturedSpline->m_interpolationType = TexturedSpline::InterpolationType::B_CUBIC;
		// Must be a power of 2 to wrap / repeat on the spline
		const float fImageSize = 64.0f;
		// But we actually want it to be 60x60, not 64x64
		// So we set it to be scaled down to 60.0f / 64.0f
		const float fSplineStrokeScale = s_kfPTM / fImageSize;
		m_pcTexturedSpline->setStrokeScale( fSplineStrokeScale );
		rcSolarLevel.addChild( m_pcTexturedSpline, -2 );
		
		const cocos2d::Vec2 v2TexturedSplineSize = m_pcTexturedSpline->getContentSize();
		m_pcTexturedSpline->setPosition( v2TexturedSplineSize * 0.5f );
	}
	else
	{
		CC_ASSERT( false );
	}
}

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

	// To avoid bad querying regarding a certain block, we can't check the entire area, as it will trigger with every adjacent block
	// So we can avoid this by querying a slightly smaller area, 10x10 instead of 60x60 for example
	// Because Querying will be done with Box2D we have to use Box2D units
	// 1 block is 1 unit, so we can set the margin to be something like 0.40f, and we'll be querying 20% of the area inside the block
	// 0.40f is 40% of the block, so if we contract the block by 40% on both sides we contracted 80%, hence the 20% area we're left with
	const float kfInwardsMargin = 0.40f;
	
	// Prepare logic for while looping
	// Light only truly stops once it hits a platform
	bool bLightHitGround = false;
	// Light goes either down or left, so a bool is enough to keep track of direction
	bool bGoingDown = true;

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

	CSolarLightB2AABBQueryCB aabbQueryCB;
	b2AABB aabbQuery;
	
	int iNumOfReflections = 0;

	while( !bLightHitGround )
	{
		// Readjust AABB Query area to be a small square inside our new current block
		aabbQuery.lowerBound = b2Vec2(	 iCurrentX + kfInwardsMargin,	  ( iCurrentY - 1 ) + kfInwardsMargin );
		aabbQuery.upperBound = b2Vec2( ( iCurrentX + 1 ) - kfInwardsMargin,	iCurrentY - kfInwardsMargin );

		// Reset existing flag on the callback
		aabbQueryCB.Reset();

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
				// We must add 3 points in case of a light bounce
				// 3 points will ensure our light beam only curves inside this block
				// Less points would make the light beam spline start shifting outside this cube
				// which would make it visually inaccurate
				if( bGoingDown )
				{
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY ) * s_kfPTM );
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY - 0.5f ) * s_kfPTM );
					m_vPath.push_back( cocos2d::Vec2( iCurrentX, iCurrentY - 0.5f ) * s_kfPTM );
				}
				else
				{
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 1.0f, iCurrentY - 0.5f ) * s_kfPTM );
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY - 0.5f ) * s_kfPTM );
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY - 1.0f ) * s_kfPTM );
				}

				// Change light direction ( flip bool )
				bGoingDown = !bGoingDown;
				CCLOG( "Hit Enemy on ( %d, %d )" , iCurrentX , iCurrentY );
				iNumOfReflections++;
			}
			break;
			// -----------------------------------------------------------------------------------------------
			// PLATFORM
			// -----------------------------------------------------------------------------------------------
			case ESolarLightQueryResult::Platform:
			{
			// If Raycast hit Platform, we're done with RTX this frame 
				bLightHitGround = true;
				CCLOG( "Reached end of path on ( %d, %d )", iCurrentX, iCurrentY );
				if (bGoingDown)
				{
					m_vPath.push_back( cocos2d::Vec2( iCurrentX + 0.5f, iCurrentY ) * s_kfPTM );
				}
				else
				{
					m_vPath.push_back( cocos2d::Vec2( iCurrentX, iCurrentY - 0.5f ) * s_kfPTM );
				}
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

	CCLOG( "%d Reflections", iNumOfReflections );
}

void CSolarLight::BuildNewSpline()
{
	// Clean up the spline's saved path to set the new path
	// For some reason this is not handled inside its buildGeometry()
	m_pcTexturedSpline->path.clear();
	m_pcTexturedSpline->path = m_vPath;

	// Rebuild spline geometry
	m_pcTexturedSpline->buildGeometry( m_vPath, 3 );
	const cocos2d::Vec2 v2TexturedSplineSize = m_pcTexturedSpline->getContentSize();
	m_pcTexturedSpline->setPosition( v2TexturedSplineSize * 0.5f );
}