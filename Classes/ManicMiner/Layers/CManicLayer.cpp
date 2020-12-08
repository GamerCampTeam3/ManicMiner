// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //
#include <string>

#include "CManicLayer.h"

#include "AppDelegate.h"

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/Win32Input/GCKeyboardManager.h"

#include "ManicMiner/AirManager/AirManager.h"
#include "ManicMiner/AudioHelper/ManicAudio.h"
#include "ManicMiner/Collectible/CCollectible.h"
#include "ManicMiner/Doors/CDoor.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "ManicMiner/Enemy/GCObjEugene.h"
#include "ManicMiner/Enemy/GCObjKong.h"
#include "ManicMiner/Enums/ELifeUpdateType.h"
#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/Hazards/GCObjHazard.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/HUD/CHUD.h"
#include "ManicMiner/LevelManager/CLevelManager.h"
#include "ManicMiner/Parallax/Parallax.h"
#include "ManicMiner/Parallax/ParallaxLayerData.h"
#include "ManicMiner/Platforms/CBrickPlatform.h"
#include "ManicMiner/Platforms/CCrumblingPlatform.h"
#include "ManicMiner/Platforms/CGroundPlatform.h"
#include "ManicMiner/Platforms/CMovingPlatform.h"
#include "ManicMiner/Platforms/CPlatform.h"
#include "ManicMiner/Platforms/CRegularPlatform.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ManicMiner/Structs/SLevelValues.h"
#include "ManicMiner/Switch/CSwitch.h"




USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// this just demos how simple it is to turn on/off logging on a define....
#define ENABLE_COLLISION_TEST_LOGGING

//#define PLAYER_DEBUG_VIRTUAL_CONTACTS

#if defined (ENABLE_COLLISION_TEST_LOGGING)

#define COLLISIONTESTLOG( str )		CCLOG( str )

#else

#define COLLISIONTESTLOG( str )		/*nothing*/

#endif


// Constructor -------------------------------------------------------------------------------------------------------- //
CManicLayer::CManicLayer()
	: IGCGameLayer( GetGCTypeIDOf( CManicLayer ) )
	, m_pcGameManager				( nullptr )
	, m_sLevelCreationParameters	( SLevelCreationParameters() )
	, m_pcLevelManager				( nullptr )
	, m_eGameState					( EGameState::Looting )
	, m_bWasResetRequested			( false )
	, m_bWasNextLevelRequested		( false )
	, m_pcParallax					( nullptr )
	, m_pcPlayer					( nullptr )
	, m_bUseHelpMenuIngame			( false   )
{
}

// Destructor  -------------------------------------------------------------------------------------------------------- //
CManicLayer::~CManicLayer()
{}

//////////////////////////////////////////////////////////////////////////
// in order to guarantee the actions this layer expects we need to 
// initialise the keyboard action map every time onEnter is called - this
// fixes the case where an IGCGameLayer with a different mapping was
// pushed over this one on Director's scene stack
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// on create
//////////////////////////////////////////////////////////////////////////
//virtual
void CManicLayer::VOnCreate()
{

	//////////////////////////////////////////////////////////////////////////
	// cache some useful values 
	m_sizeVisible = Director::getInstance()->getVisibleSize();
	m_pointOrigin = Director::getInstance()->getVisibleOrigin();


	// create the default object group
	IGCGameLayer::VOnCreate();

	///////////////////////////////////////////////////////////////////////////
	/// Exit / Help Button
	// Set to false to return the button so it's original implementation:
	// Goes to next level
	// If True, a pause menu will appear.
	m_bUseHelpMenuIngame = true;
	CreateExitButton();
	///////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////
	// set up physics 
	///////////////////////////////////////////////////////////////////////////

	// set "self" as contact listener
	B2dGetWorld()->SetContactListener( this );

	// load the physics shapes from the plist created with PhysicsEditor
	B2dLoadShapesFromPlist( "PhysicsEditor/GameShapes.plist" );

	// PTM_RATIO
	f32 PTM_RATIO = IGCGAMELAYER_B2D_PIXELS_PER_METER;

	b2Vec2	b2v2ScreenCentre_Pixels( (m_pointOrigin.x + (m_sizeVisible.width * 0.5f ) ), (m_pointOrigin.y + (m_sizeVisible.height * 0.5f ) ) );


	// define the ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position = IGCGameLayer::B2dPixelsToWorld( b2v2ScreenCentre_Pixels );
	groundBodyDef.type = b2_kinematicBody;

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = B2dGetWorld()->CreateBody( &groundBodyDef );

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// bottom
	groundBox.SetAsBox( ( (m_sizeVisible.width * 0.5f ) / PTM_RATIO ), 0.5f, b2Vec2( 0.0f, -( (m_sizeVisible.height * 0.5f ) / PTM_RATIO ) ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// top
	groundBox.SetAsBox( ( (m_sizeVisible.width * 0.5f ) / PTM_RATIO ), 0.5f, b2Vec2( 0.0f, ( (m_sizeVisible.height * 0.5f ) / PTM_RATIO ) ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// left
	groundBox.SetAsBox( 0.5f, ( (m_sizeVisible.height * 0.5f ) / PTM_RATIO ), b2Vec2( -( (m_sizeVisible.width * 0.5f ) / PTM_RATIO ), 0.0f ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// right
	groundBox.SetAsBox( 0.5f, ( (m_sizeVisible.height * 0.5f ) / PTM_RATIO ), b2Vec2( ( (m_sizeVisible.width * 0.5f ) / PTM_RATIO ), 0.0f ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );


	///////////////////////////////////////////////////////////////////////////
	// N.B. this is where you would load a level file, using the factory to
	// create the class instances specified by the level file by passing their
	// class names and init data
	//
	// In order to ensure stuff is correctly cleaned up you will need to either:
	//
	// 1) cache pointers to all created CGCObjSpritePhysics created via the factory and destroy them on shutdown / level unload, or
	//
	// 2) make sure all the CGCObjSpritePhysics derived instances you create are in an object group and have the groups destroy them
	//
	// Personally I favour option 1, as I reckon it's a) more elegant and b) more philosophically 'correct'.
	//
	///////////////////////////////////////////////////////////////////////////
	
	// If the level fails to receive a string or the string provided does not contain at least Ogmoeditor
	// then fail
	if ( (m_sLevelCreationParameters.szLevelPath._Equal( "" ) != 0 ) || (m_sLevelCreationParameters.szLevelPath.find( "OgmoEditor/" ) != std::string::npos) )
	{
		m_cLevelLoader.LoadLevelFile( FileUtils::getInstance()->fullPathForFilename( std::string( m_sLevelCreationParameters.szLevelPath ) ).c_str() );
		m_cLevelLoader.CreateObjects( CGCFactory_ObjSpritePhysics::GetFactory() );
	}


	///////////////////////////////////////////////////////////////////////////
	// add player
	///////////////////////////////////////////////////////////////////////////
	m_pcPlayer = new CPlayer( *this, m_sLevelCreationParameters.v2PlayerStartPos, m_sLevelCreationParameters.bShouldFaceRight );

	// ------------------------------------------------------------------------------------------------------------------------------------------------ //


	GetCollisionManager().AddCollisionHandler( [&] ( CPlayer& rcPlayer, CCollectible& rcCollectible, const b2Contact& rcContact ) -> void
		{
			ItemCollected( rcCollectible, rcPlayer, rcContact );
		} );

	GetCollisionManager().AddCollisionHandler( [&]( CPlayer& rcPlayer, CSwitch& rcSwitch, const b2Contact& rcContact ) -> void
	{
		SwitchInteracted( rcSwitch, rcPlayer, rcContact );
	} );

	GetCollisionManager().AddCollisionHandler( [&] ( CPlayer& rcPlayer, CGCObjEnemy& rcEnemy, const b2Contact& rcContact ) -> void
		{
			PlayerCollidedEnemy( rcPlayer, rcEnemy, rcContact );
		} );

	GetCollisionManager().AddCollisionHandler([&](CPlayer& rcPlayer, CGCObjEugene& rcEugene, const b2Contact& rcContact) -> void
		{
			PlayerCollidedEnemy(rcPlayer, rcEugene, rcContact);
		});
	   	 
	GetCollisionManager().AddCollisionHandler( [&] (CPlayer& rcPlayer, CGCObjHazard& rcHazard, const b2Contact& rcContact) -> void
		{
			PlayerCollidedHazard(rcPlayer, rcHazard, rcContact);
		});

	GetCollisionManager().AddCollisionHandler( [&] (CPlayer& rcPlayer, CDoor& rcDoor, const b2Contact& rcContact) -> void
		{
			PlayerCollidedDoor( rcPlayer, rcDoor, rcContact );
		});

	GetCollisionManager().AddCollisionHandler( [&]( CPlayer& rcPlayer, CGCObjKong& rcKong, const b2Contact& rcContact ) -> void
	{
		PlayerCollidedEnemy( rcPlayer, rcKong, rcContact );
	} );

}

void CManicLayer::VInitializeBackground()
{
	auto pcScene = static_cast< cocos2d::Scene* >( getParent() );
	if ( pcScene && m_pcPlayer )
	{
		switch( m_sLevelCreationParameters.eParallaxTheme )
		{

		case EParallaxTheme::Cavern:
		{
			int kiNumScrollingLayers = 3;
			const int kiNumAnimatedLayers = 1;
			m_pcParallax = new CParallax( kiNumScrollingLayers, kiNumAnimatedLayers, *pcScene, *m_pcPlayer );

			const SParallaxLayerData sData1( "TexturePacker/Backgrounds/Cavern/Background_0.plist", -3 );
			const SParallaxLayerData sData2( "TexturePacker/Backgrounds/Cavern/Background_1.plist", -5 );
			const SParallaxLayerData sData3( "TexturePacker/Backgrounds/Cavern/CavernForeground.plist", 1 );

			m_pcParallax->AddScrollingLayer( sData1, 1.0f );
			m_pcParallax->AddScrollingLayer( sData2, 0.025f );
			m_pcParallax->AddScrollingLayer( sData3, 40.5f );
			
			
			const SParallaxLayerData sData4( "TexturePacker/Sprites/Fish/Fish.plist", -4 );
			m_pcParallax->AddAnimatedLayer(sData4);
		}
		break;
		case EParallaxTheme::Toxic:
		{
			int kiNumScrollingLayers = 3;
			const int kiNumAnimatedLayers = 0;
			m_pcParallax = new CParallax( kiNumScrollingLayers, kiNumAnimatedLayers, *pcScene, *m_pcPlayer );

			const SParallaxLayerData sData1( "TexturePacker/Backgrounds/Toxic/Background.plist", -10 );
			const SParallaxLayerData sData2( "TexturePacker/Backgrounds/Toxic/Outline.plist", -2 );
			const SParallaxLayerData sData3( "TexturePacker/Backgrounds/Toxic/Fog.plist", -7 );

			m_pcParallax->AddScrollingLayer( sData1, 0.0f );
			m_pcParallax->AddScrollingLayer( sData2, 0.0f );
			m_pcParallax->AddScrollingLayer( sData3, 5.0f );
		}
		break;
		case EParallaxTheme::Shipwreck:
		{
			int kiNumScrollingLayers = 5;
			const int kiNumAnimatedLayers = 0;
			m_pcParallax = new CParallax( kiNumScrollingLayers, kiNumAnimatedLayers, *pcScene, *m_pcPlayer );

			const SParallaxLayerData sData1( "TexturePacker/Backgrounds/Shipwreck/ShipwreckBackground.plist", -10 );
			const SParallaxLayerData sData2( "TexturePacker/Backgrounds/Shipwreck/ShipwreckChains.plist", -9 );
			const SParallaxLayerData sData3( "TexturePacker/Backgrounds/Shipwreck/ShipwreckTop.plist", -8 );
			const SParallaxLayerData sData4( "TexturePacker/Backgrounds/Shipwreck/ShipwreckBoundaries.plist", -7 );
			const SParallaxLayerData sData5( "TexturePacker/Backgrounds/Shipwreck/ForegroundShipwreck.plist", 1 );

			m_pcParallax->AddScrollingLayer( sData1, 0.2f );
			m_pcParallax->AddScrollingLayer( sData2, 0.0f );
			m_pcParallax->AddScrollingLayer( sData3, 0.0f );
			m_pcParallax->AddScrollingLayer( sData4, 0.0f );
			m_pcParallax->AddScrollingLayer( sData5, 4.0f );
		}
		break;
		case EParallaxTheme::Magma:
		{
			int kiNumScrollingLayers = 1;
			const int kiNumAnimatedLayers = 0;
			m_pcParallax = new CParallax( kiNumScrollingLayers, kiNumAnimatedLayers, *pcScene, *m_pcPlayer );

			const SParallaxLayerData sData1( "TexturePacker/Backgrounds/Lava/LavaBackground.plist", -10 );

			m_pcParallax->AddScrollingLayer( sData1, 0.0f );
		}
		break;
		}

	m_pcParallax->Reset();
	}


	if (m_sLevelCreationParameters.pszPlatformBackground != nullptr)
	{
		GetParallax().AddPlatformOutlines( m_sLevelCreationParameters.pszPlatformBackground );
	}
}


//////////////////////////////////////////////////////////////////////////
// on update
//////////////////////////////////////////////////////////////////////////
//virtual		
void CManicLayer::VOnUpdate( f32 fTimeStep )
{
	IGCGameLayer::VOnUpdate( fTimeStep );

	if ( m_pcParallax )
	{
		m_pcParallax->Update();
	}

	// Reset level request ( and not going to next level )
	if( GetWasResetRequested() && !GetWasNextLevelRequested() )
	{
		ResetLevel();
		CCLOG( "Level Got Reset" );
	}

	// Go to next level if requested
	if( GetWasNextLevelRequested() )
	{
		m_bWasNextLevelRequested = false;
		m_pcLevelManager->GoToNextLevel();
	}
}

///////////////////////////////////////////////////////////////////////////////
// on destroy
///////////////////////////////////////////////////////////////////////////////
// virtual				
void CManicLayer::VOnDestroy()
{
	///////////////////////////////////////////////////////////////////////////
	// clean up anything we allocated in opposite order to creation
	///////////////////////////////////////////////////////////////////////////

	// Clean up the level
	m_cLevelLoader.DestroyObjects();
	
	// Delete Parallax
	//if ( m_pzsPlatformOutlinePlist )
	//{
	//	delete m_pzsPlatformOutlinePlist;
	//	m_pzsPlatformOutlinePlist = nullptr;
	//}
	if ( m_pcParallax ) 
	{
		delete m_pcParallax;
		m_pcParallax = nullptr;
	}

	
	IGCGameLayer::VOnDestroy();
}



void CManicLayer::VOnReset( void )
{
	IGCGameLayer::VOnReset();
	m_pcGameManager->ResetEvent();
}
// --- b2ContactListener Interface ------------------------------------------------------------------------------------ //
																														//
// BeginContact happens for the first frame of a unique collision														//
// EndContact happens for the last frame of a unique collision															//
																														//
// We use these 2 functions to detect if the collision is between the player and a platform								//
// If so, we either add or subtract to the number of platform collisions the player is detecting during this 1 frame	//
// With that info, we know if the player is grounded or if he is in mid air												//
																														//
// This also allows us to detect sensor fixture overlaps, which we need to activate and deactivate						//
// the collision on certain platforms in order for the player to jump and pass through them								//
																														//
void CManicLayer::BeginContact( b2Contact* pB2Contact )																	//
{																														//
	// Get pointers to both b2fixtures																					//
	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();																//
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();																//
																														//
	// Get pointers to both b2bodies																					//
	const b2Body* pBodyA = pFixtureA->GetBody();																		//
	const b2Body* pBodyB = pFixtureB->GetBody();																		//
																														//
	CGCObjSpritePhysics* pGcSprPhysA = (CGCObjSpritePhysics*)pBodyA->GetUserData();										//
	// if( this is not a GC object )																					//
	if (pGcSprPhysA == nullptr)																							//
	{																													//
		return;																											//
	}																													//
																														//
	CGCObjSpritePhysics* pGcSprPhysB = (CGCObjSpritePhysics*)pBodyB->GetUserData();										//
	// if( this is not a GC object )																					//
	if (pGcSprPhysB == nullptr)																							//
	{																													//
		return;																											//
	}																													//
																														//
																														//
	// Henrique Edit - Player Platforming Abilities																		//
	// CHECK FOR PLATORM AND PLAYER COLLISION																			//
	if (pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID())														//
	{																													//
		if (((pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ))													//
			&& (pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlatform )))												//
			|| ((pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlatform ))												//
			&& (pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlayer ))))												//
		{																												//
																														//
			// Get pointer to platform																					//
			CPlatform* pcPlatform = nullptr;																			//
																														//
			// If BodyA is player, BodyB must be platform																//
			if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )												//
			{																											//
				pcPlatform = static_cast< CPlatform* >( pGcSprPhysB );													//
			}																											//
			// Else, vice versa																							//
			else																										//
			{																											//
				pcPlatform = static_cast< CPlatform* >( pGcSprPhysA );													//
			}																											//
																														//
																														//
																														//
			// Check if this BeginContact is for feet + platform surface ( sensors only )								//
			if( pFixtureA->IsSensor() && pFixtureB->IsSensor() )														//
			{																											//
				if( pcPlatform->GetPlatformType() != EPlatformType::Brick )
				{
					// GET ID NAME
					const std::string* pszSensorIdA = GB2ShapeCache::getFixtureIdText( pFixtureA );
					auto pszSensorIdB = GB2ShapeCache::getFixtureIdText( pFixtureB );

					// Check if this platform was already overlapping in terms of hard contact
					// && if collision is not yet enabled
					// && if platform is not triggering a hard contact yet
					// && if the player is not moving upwards ( otherwise it would trigger when player jumps through a platform, landing on that platform for just a frame and then moving upwards anyways )
					bool bShouldStartHardContact = ( pcPlatform->GetIsInContact() && !pcPlatform->GetCollisionEnabled() ) && !pcPlatform->GetIsTriggeringHardContact() && m_pcPlayer->GetVelocity().y <= 0.0f;

					// Activate this platform's collision																	//
					pcPlatform->SetCollisionEnabled( true );																//
																															//
																															//
					// Increment sensor count for the player																//
					m_pcPlayer->SensorContactEvent( true );																	//

					pcPlatform->SetIsSensorOverlapped( true );
				
					if( bShouldStartHardContact )
					{
						CCLOG( "Forcing a ghosted hard contact start, due to sensor contact now but hard fixtures were already overlapping." );
						PlayerBeganContactWithPlatform( *pcPlatform );
					}
				}
				// Brick Platform sensor overlap
				else
				{
					auto pBrickPlatform = static_cast< CBrickPlatform* > ( pcPlatform );

					
					// We have more than 1 sensor for the bricks, so we need to know which one we're overlapping with
					// We have the surface sensor, used to determine whether this brick is below the player or not
					// And we have 2 side sensors that allow the player to climb up this ledge
					// GET ID NAME
					const std::string* pszSensorIdA = GB2ShapeCache::getFixtureIdText( pFixtureA );
					auto pszSensorIdB = GB2ShapeCache::getFixtureIdText( pFixtureB );
					if( ( *pszSensorIdB == "surface" && *pszSensorIdA == "brickFeet" ) ||
						( *pszSensorIdB == "brickFeet" && *pszSensorIdA == "surface" ) )
					{
						pBrickPlatform->SetIsUnderPlayer( true );

						// Increment sensor count for the player																//
						m_pcPlayer->SensorContactEvent( true );																	//

						pBrickPlatform->SetIsSensorOverlapped( true );
					}

				}
			}																											//
																														//
			// If not 2 sensors																							//
			// Check if this BeginContact is for character + platform ( no sensors included )							//
			else if( !( pFixtureA->IsSensor() ) && !( pFixtureB->IsSensor() ) )											//
			{																											//
				PlayerBeganContactWithPlatform( *pcPlatform );
			}

			// Exclusive Or
			// BeginContact triggered with hard fixture + sensor fixture
			// This is an edge case we need to check for in order to prevent a bug with the bricks platform
			// This type of platform by default has collision on so the player might land on the top without
			// having overlapped sensors first.
			//else if( pFixtureA->IsSensor() ^ pFixtureB->IsSensor() )
			//{
			//	if( pcPlatform->GetPlatformType() == EPlatformType::Brick )
			//	{
			//		// GET ID NAME
			//		auto pszSensorIdA = GB2ShapeCache::getFixtureIdText( pFixtureA );
			//		auto pszSensorIdB = GB2ShapeCache::getFixtureIdText( pFixtureB );
			//		if( (		*pszSensorIdA == "surface"	&& *pszSensorIdB == "body" ) 
			//			|| (	*pszSensorIdA == "body"		&& *pszSensorIdB == "surface" ) )
			//		{
			//			auto pBrickPlatform = static_cast< CBrickPlatform* > ( pcPlatform );
			//			if ( pBrickPlatform != nullptr )
			//			{
			//				pBrickPlatform->SetIsUnderPlayer( true );
			//			}
			//		}
			//	}
			//}
		}																												//
	}																													//
}																														//
																														//
void CManicLayer::EndContact( b2Contact* pB2Contact )																	//
{																														//
	// Get pointers to both b2fixtures																					//
	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();																//
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();																//
																														//
	// Get pointers to both b2bodies																					//
	const b2Body* pBodyA = pFixtureA->GetBody();																		//
	const b2Body* pBodyB = pFixtureB->GetBody();																		//
																														//
	CGCObjSpritePhysics* pGcSprPhysA = ( CGCObjSpritePhysics* )pBodyA->GetUserData();									//
	// if( this is not a GC object )																					//
	if( pGcSprPhysA == nullptr )																						//
	{																													//
		return;																											//
	}																													//
																														//
	CGCObjSpritePhysics* pGcSprPhysB = ( CGCObjSpritePhysics* )pBodyB->GetUserData();									//
	// if( this is not a GC object )																					//
	if( pGcSprPhysB == nullptr )																						//
	{																													//
		return;																											//
	}																													//
																														//
																														//
																														//
	// Henrique Edit - Player Platforming Abilities																		//
	// CHECK FOR PLATORM AND PLAYER COLLISION																			//
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )														//
	{																													//
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )												//
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) ) )											//
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) )											//
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) ) ) )										//
		{																												//
			// Get pointer to platform																					//
			CPlatform* pcPlatform = nullptr;																				//
																														//
			// If BodyA is player, BodyB must be platform																//
			if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )												//
			{																											//
				pcPlatform = static_cast< CPlatform* >( pGcSprPhysB );													//
			}																											//
			// Else, vice versa																							//
			else																										//
			{																											//
				pcPlatform = static_cast< CPlatform* >( pGcSprPhysA );													//
			}																											//
																														//
																														//
																														//
			// Check if this EndContact is for feet + platform surface sensors only										//
			if( pFixtureA->IsSensor() && pFixtureB->IsSensor() )														//
			{																											//
				// If this platform is not a CBrickPlatform																//
				if( pcPlatform->GetPlatformType() != EPlatformType::Brick )												//
				{																										//
					// Deactivate this platform's collision																//
					pcPlatform->SetCollisionEnabled( pcPlatform->GetTriggersHardContactEvent() );						//

					// Decrement sensor contact count																	//
					m_pcPlayer->SensorContactEvent( false );															//

					pcPlatform->SetIsSensorOverlapped( false );
				}																										//
				// Brick Platform sensor overlap end
				else
				{
					auto pBrickPlatform = static_cast< CBrickPlatform* > ( pcPlatform );


					// We have more than 1 sensor for the bricks, so we need to know which one we're overlapping with
					// We have the surface sensor, used to determine whether this brick is below the player or not
					// And we have 2 side sensors that allow the player to climb up this ledge
					// GET ID NAME
					const std::string* pszSensorIdA = GB2ShapeCache::getFixtureIdText( pFixtureA );
					auto pszSensorIdB = GB2ShapeCache::getFixtureIdText( pFixtureB );
					if( ( *pszSensorIdB == "surface"   && *pszSensorIdA == "brickFeet" ) ||
						( *pszSensorIdB == "brickFeet" && *pszSensorIdA == "surface" ) )
					{
						pBrickPlatform->SetIsUnderPlayer( false );
						// Increment sensor count for the player
						m_pcPlayer->SensorContactEvent( false );

						pBrickPlatform->SetIsSensorOverlapped( false );
					}
					else if( ( *pszSensorIdA == "ClimbUpLedge"	&& *pszSensorIdB == "feet" )
						  || ( *pszSensorIdA == "feet"			&& *pszSensorIdB == "ClimbUpLedge" ) )
					{
						m_pcPlayer->ClimbUpBrickLedge();
					}
				}
			}																											//
																														//
			// If not 2 sensors																							//
			// Check if this EndContact is for character + platform ( no sensors included )								//
			else if( !( pFixtureA->IsSensor() ) && !( pFixtureB->IsSensor() ) )											//
			{																											//
				if( pcPlatform->GetTriggersHardContactEvent() )															//
				{																										//
					// Set the platform as no trigger for hard contact events											//
					pcPlatform->SetTriggersHardContactEvent( false );													//
																														//
					// Decrement hard contact count																		//
					m_pcPlayer->HardContactEvent( false );																//

					pcPlatform->SetIsTriggeringHardContact( false );
																														//

					// If feet are no longer touching any ground surface												//
					if( !m_pcPlayer->GetHardContactCount() && m_pcPlayer->GetIsGrounded() )								//
					{																									//
						m_pcPlayer->LeftGround();																		//
					}																									//
					// If this was a Crumbling platform, stop crumbling													//
					switch( pcPlatform->GetPlatformType() )																//
					{																									//
						case EPlatformType::Crumbling:																	//
						{						
							auto pcCrumblingPlatform = static_cast< CCrumblingPlatform* > ( pcPlatform );				//
																														//
							if( pcCrumblingPlatform != nullptr )														//
							{																							//
								pcCrumblingPlatform->StopCrumbling();													//
							}																							//
						}
						break;																							//
					}																									//
					// If leaving this contact, and sensors aren't overlapping anymore as well
					if ( !pcPlatform->GetIsSensorOverlapped() && pcPlatform->GetPlatformType() != EPlatformType::Brick )
					{
						// Disable platform collision
						pcPlatform->SetCollisionEnabled( false );
					}
				}																										//
				else
				{
#ifdef PLAYER_DEBUG_VIRTUAL_CONTACTS
					CCLOG( "ENDED SOLID CONTACT WITH A PLATFORM THAT HAS COLLISION OFF" );
#endif
				}
				pcPlatform->SetIsInContact( false );
			}																											//

			//// Exclusive Or
			//// EndContact triggered with hard fixture + sensor fixture
			//// This is an edge case we need to check for in order to prevent a bug with the bricks platform
			//// This type of platform by default has collision on so the player might land on the top without
			//// having overlapped sensors first.
			//else if( pFixtureA->IsSensor() ^ pFixtureB->IsSensor() )
			//{
			//	if( pcPlatform->GetPlatformType() == EPlatformType::Brick )
			//	{
			//		// GET ID NAME
			//		auto pszSensorIdA = GB2ShapeCache::getFixtureIdText( pFixtureA );
			//		auto pszSensorIdB = GB2ShapeCache::getFixtureIdText( pFixtureB );
			//		if( ( *pszSensorIdA == "surface" && *pszSensorIdB == "body" )
			//			|| ( *pszSensorIdA == "body" && *pszSensorIdB == "surface" ) )
			//		{
			//			auto pBrickPlatform = static_cast< CBrickPlatform* > ( pcPlatform );
			//			if( pBrickPlatform != nullptr )
			//			{
			//				pBrickPlatform->SetIsUnderPlayer( false );
			//			}
			//		}
			//		else 
			//		
			//		
			//		{
			//			if( m_pcPlayer->GetVelocity().y > 0.0f )
			//			{
			//			}
			//		}
			//	}
			//}
		}																												//
	}																													//
}																														//
																														//
// PreSolve happens once every frame for every collision detected														//
// It runs before the physics engine does any calculations, so it is a great place to ignore collisions effectively		//
// This way we ensure we bypass the physics engine for certain collisions												//
// specifically Player & Pass-Through enabled Platforms																	//
																														//
void CManicLayer::PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold )										//
{																														//
	// Get pointers to both b2fixtures																					//
	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();																//
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();																//
																														//
	// Get pointers to both b2bodies																					//
	const b2Body* pBodyA = pFixtureA->GetBody();																		//
	const b2Body* pBodyB = pFixtureB->GetBody();																		//
																														//
	CGCObjSpritePhysics* pGcSprPhysA = ( CGCObjSpritePhysics* )pBodyA->GetUserData();									//
	// if( this is not a GC object )																					//
	if( pGcSprPhysA == nullptr )																						//
	{																													//
		return;																											//
	}																													//
																														//
	CGCObjSpritePhysics* pGcSprPhysB = ( CGCObjSpritePhysics* )pBodyB->GetUserData();									//
	// if( this is not a GC object )																					//
	if( pGcSprPhysB == nullptr )																						//
	{																													//
		return;																											//
	}																													//
																														//
	// Henrique Edit - Player Platforming Abilities																		//
	// CHECK FOR PLATORM AND PLAYER COLLISION																			//
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )														//
	{																													//
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )												//
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) ) )											//
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) )											//
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) ) ) )										//
		{																												//
			// Check if this BeginContact is not running with any sensor triggers										//
			if( !( pFixtureA->IsSensor() ) && !( pFixtureB->IsSensor() ) )												//
			{																											//
				// We must check if this platform is supposed to have collision or not									//
																														//
				// Get pointer to platform																				//
				CPlatform* pcPlatform = nullptr;																			//
																														//
				// If BodyA is player, BodyB must be platform															//
				if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )											//
				{																										//
					pcPlatform = static_cast< CPlatform* >( pGcSprPhysB );												//
				}																										//
				// Else, vice versa																						//
				else																									//
				{																										//
					pcPlatform = static_cast< CPlatform* >( pGcSprPhysA );												//
				}																										//
																														//
				// Set contact collision accordingly																	//
				pB2Contact->SetEnabled( pcPlatform->GetCollisionEnabled() );												//
			}																											//
																														//
		}																												//
	}																													//
}																														//
// -------------------------------------------------------------------------------------------------------------------- //


// ---- Object Specific Collision Handles ----------------------------------------------------------------------------- //
																														//
void CManicLayer::EnemyCollidedPlatform( CGCObjEnemy& rcEnemy, const b2Contact& rcContact )								//
{																														//
	if( rcContact.IsTouching() )																						//
	{																													//
		//rcEnemy.BounceEnemyDirection();																					//
	}																													//
}																														//
																														//
																														//
void CManicLayer::PlayerCollidedEnemy( CPlayer& rcPlayer, CGCObjEnemy& rcEnemy, const b2Contact& rcContact )			//
{																														//
	if( rcContact.IsTouching() )																						//
	{																													//
		OnDeath();
	}																													//
}

void CManicLayer::PlayerCollidedEnemy( CPlayer& rcPlayer, CGCObjKong& rcKong, const b2Contact& rcContact )			//
{																														//
	if (rcContact.IsTouching())																						//
	{																													//
		OnDeath();
	}																													//
}			//
																														//
																														//
void CManicLayer::PlayerCollidedHazard( CPlayer& rcPlayer, CGCObjHazard& rcHazard, const b2Contact& rcContact )			//
{																														//
	if( rcContact.IsTouching() )																						//
	{
		if (rcHazard.GetCanCollide() )
		{

			OnDeath();
			rcHazard.SetCanCollide( false );
		}

	}																													//
}																														//
																														//
																														//
void CManicLayer::PlayerCollidedDoor( CPlayer& rcPlayer, CDoor& rcDoor, const b2Contact& rcContact )					//
{																														//
	if ( rcContact.IsTouching() )																						//
	{																													//
		m_pcGameManager->EndLevel();
	}																													//
}																														//
																														//
																														//
void CManicLayer::ItemCollected( CCollectible& rcCollectible, CPlayer& rcPlayer, const b2Contact& rcContact )			//
{																														//
	if( rcContact.IsTouching() )																						//
	{																													//
		rcCollectible.InteractEvent();																					//
		m_pcGameManager->CCollectibleInteractEvent();
	}																													//
}																														//
																														//
void CManicLayer::SwitchInteracted(CSwitch& rcSwitch, CPlayer& rcPlayer, const b2Contact& rcContact)					//
{																														//
	if (rcContact.IsTouching())																							//
	{
		if (!rcSwitch.GetHasBeenCollected())
		{
			m_pcGameManager->CSwitchInteractEvent();
		}
		
		rcSwitch.InteractEvent();

	}																													//
}																														//

CAirManager& CManicLayer::GetAirManager() const
{
	return *m_pcAirManager;
}

// -------------------------------------------------------------------------------------------------------------------- //



// ---------------------------------- Getters ------------------------------------------------------------------------- //
																														//
CPlayer& CManicLayer::GetPlayer() const																					//
{																														//
	return *m_pcPlayer;																									//
}																														//
																														//
																														//
CLevelManager& CManicLayer::GetLevelManager() const																		//
{																														//
	return *m_pcLevelManager;																							//
}																														//
																														//
																														//
const EGameState CManicLayer::GetGameState() const																		//
{																														//
	return m_eGameState;																								//
}																														//
																														//
																														//
bool CManicLayer::GetWasResetRequested() const																			//
{																														//
	return m_bWasResetRequested;																						//
}																														//
																														//
																														//
bool CManicLayer::GetWasNextLevelRequested() const																		//
{																														//
	return m_bWasNextLevelRequested;																					//
}																														//

CParallax& CManicLayer::GetParallax() const
{
	CC_ASSERT( m_pcParallax != nullptr );
	return *m_pcParallax;
}

// -------------------------------------------------------------------------------------------------------------------- //




// ---------------------------------- Setters ------------------------------------------------------------------------- //
void CManicLayer::SetLevelManager( CLevelManager& rcLevelManager )														//
{																														//
	m_pcLevelManager = &rcLevelManager;																					//
}																														//
																														//
																														//
void CManicLayer::SetGameState( const EGameState gameState ) 															//
{ 																														//
	m_eGameState = gameState;																							//
}																														//
																														//
void CManicLayer::SetGameManager(CGameManager& rcGameManager)															//
{																														//
	m_pcGameManager = &rcGameManager;																					//
}																														//
// -------------------------------------------------------------------------------------------------------------------- //


void CManicLayer::onEnter()
{
	IGCGameLayer::onEnter();

	//////////////////////////////////////////////////////////////////////////
	// init the actions
	// N.B. these 
	cocos2d::EventKeyboard::KeyCode aeKeyCodesForActions[] =
	{
		EventKeyboard::KeyCode::KEY_UP_ARROW,		// EPA_Up,
		EventKeyboard::KeyCode::KEY_DOWN_ARROW,		// EPA_Down,
		EventKeyboard::KeyCode::KEY_LEFT_ARROW,		// EPA_Left,		
		EventKeyboard::KeyCode::KEY_RIGHT_ARROW,	// EPA_Right,
		EventKeyboard::KeyCode::KEY_H,				// EPA_Cheat,
		EventKeyboard::KeyCode::KEY_SPACE,			// EPA_Fire	
	};

	u32 uSizeOfActionArray = ( sizeof( aeKeyCodesForActions ) / sizeof( cocos2d::EventKeyboard::KeyCode ) );

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager( uSizeOfActionArray, aeKeyCodesForActions );

	m_pcGameManager->CheckShouldUpdateMusic( m_sLevelCreationParameters.eBackgroundMusic );

}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnFinishedLooting																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CManicLayer::OnFinishedLooting()
{
	m_eGameState = EGameState::Escaping;
	// For production phase
	// Change enemy behaviours if needed
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnEscaped																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CManicLayer::OnEscaped()
{
	m_eGameState = EGameState::Victory;
	// For production phase
	// Run Time animation and points, etc
	// when that ends, call this line:
	RequestNextLevel();
	
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OnDeath																								//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CManicLayer::OnDeath()
{
	m_pcPlayer->Die();
	m_pcGameManager->UpdateLives( ELifeUpdateType::Minus );
	m_pcGameManager->ResetInteractionCounter();

	if( m_pcPlayer->GetLives() > 0 )
	{
		RequestReset();
		
	}
	else
	{
		OutOfLives();
	}
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	OutOfLives																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CManicLayer::OutOfLives()
{
	m_pcGameManager->ResetHUD();
	m_pcGameManager->ResetLives();		
	m_pcLevelManager->GoToGameOverScene();	
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	RequestReset																						//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CManicLayer::RequestReset()
{
	m_bWasResetRequested = true;
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ResetRequestWasHandled																				//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CManicLayer::ResetRequestWasHandled()
{
	m_bWasResetRequested = false;
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	ResetLevel																							//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CManicLayer::ResetLevel()
{
	ResetRequestWasHandled();
	VOnReset();
}


// -------------------------------------------------------------------------------------------------------------------- //
// Function		:	RequestNextLevel																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Parameters	:	none																								//
//																														//
// Returns		:	void																								//
// -------------------------------------------------------------------------------------------------------------------- //
void CManicLayer::RequestNextLevel()
{
	m_bWasNextLevelRequested = true;
	m_pcGameManager->ResetHUD();
	m_pcGameManager->ResetValues();
}


void CManicLayer::PlayerBeganContactWithPlatform( CPlatform& rcPlatform )
{
	if( rcPlatform.GetCollisionEnabled() )																	//
	{																										//
		// Set the platform as a trigger for hard contact events											//
		rcPlatform.SetTriggersHardContactEvent( true );														//
																											//
		// Increment hard contact count for the player														//
		m_pcPlayer->HardContactEvent( true );																//
		rcPlatform.SetIsTriggeringHardContact( true );
																											//																														//
		// Check Platform Type																				//
		switch( rcPlatform.GetPlatformType() )																//
		{																									//
		////////////////////////////////////////////////////////////////////////////////					//
		// MOVING																	////					//
		////////////////////////////////////////////////////////////////////////////////					//
		case EPlatformType::Moving:																			//
		{																									//
			// Downcast platform to CMovingPlatform, in order to get its respective DirectionLock 			//
			auto pMovingPlatform = static_cast< CMovingPlatform* > ( &rcPlatform );							//
																											//
			if( pMovingPlatform != nullptr )																//
			{																								//
				m_pcPlayer->LandedOnConveyorBelt( pMovingPlatform->GetDirectionLock() );					//
			}																								//
		}																									//
		break;																								//
		////////////////////////////////////////////////////////////////////////////////					//
		// CRUMBLING																////					//
		////////////////////////////////////////////////////////////////////////////////					//
		case EPlatformType::Crumbling:																		//
		{																									//
				// Start Crumbling																			//
			auto pCrumblingPlatform = static_cast< CCrumblingPlatform* > ( &rcPlatform );					//
																											//
			if( pCrumblingPlatform != nullptr )																//
			{																								//
				pCrumblingPlatform->InitiateCrumbling();													//
			}																								//
																											//
				// Set player as grounded																	//
			m_pcPlayer->LandedOnWalkablePlatform();															//
		}																									//
		break;																								//
		////////////////////////////////////////////////////////////////////////////////					//
		// BRICK																	////					//
		////////////////////////////////////////////////////////////////////////////////					//
		case EPlatformType::Brick:																			//
		{																									//
			// If in mid air and sensor contacts == 0, or if on top of conveyor belt platform				//

			auto pBrickPlatform = static_cast< CBrickPlatform* > ( &rcPlatform );							//
			if( pBrickPlatform != nullptr )
			{
				if( pBrickPlatform->GetIsUnderPlayer() )
				{
					m_pcPlayer->LandedOnWalkablePlatform();													//
				}
				else
				{
					// Set the platform as not a trigger for hard contact events										//
					rcPlatform.SetTriggersHardContactEvent( false );													//
																														//
					// Decrement hard contact count for the player, because by default the platforms increment			//
					// We dont consider a side bump a "hard contact" as it's not relevant for feet and player movement mostly
					m_pcPlayer->HardContactEvent( false );																//
					rcPlatform.SetIsTriggeringHardContact( false );

					m_pcPlayer->BumpedWithBricks();															//
				}
			}
			//if( ( !m_pcPlayer->GetIsGrounded() ) && ( m_pcPlayer->GetSensorContactCount() == 0 )			//
			//	|| ( m_pcPlayer->GetIsOnConveyorBelt() ) )													//
			//{																								//
			//	// Player Bumped onto brick																	//
			//}																								//
			//// Else, is in mid-air? If so, land on platform													//
			//else if( !m_pcPlayer->GetIsGrounded() )															//
			//{																								//
			//	// Set player as grounded																	//
			//	m_pcPlayer->LandedOnWalkablePlatform();														//
			//}																								//
		}																									//
		break;																								//
		////////////////////////////////////////////////////////////////////////////////					//
		// DEFAULT																	////					//
		////////////////////////////////////////////////////////////////////////////////					//
		default:																							//
		{																									//
			// Set player as grounded																		//
			m_pcPlayer->LandedOnWalkablePlatform();															//
		}																									//
		break;																								//
		}																									//
	}																										//
	else
	{
#ifdef PLAYER_DEBUG_VIRTUAL_CONTACTS
		CCLOG( "BEGAN SOLID CONTACT WITH A PLATFORM THAT HAS COLLISION OFF" );
#endif
	}
	rcPlatform.SetIsInContact( true );
}



void CManicLayer::CB_OnGameExitButton( Ref* pSender )
{
	static CGCObjSprite* pcSprite = nullptr;
	
	
	if (m_bUseHelpMenuIngame)
	{
		// If it's not paused, pause
		if (!cocos2d::Director::getInstance()->isPaused())
		{

			cocos2d::Director::getInstance()->pause();			
			pcSprite = EasySprite::Create( "Loose/pause_menu.plist", 4.0f, 1.0f, 1.0f, CC_V2( (m_sizeVisible.width / 2 + m_pointOrigin.x), (m_sizeVisible.height / 2 + m_pointOrigin.y) ), this );
			pcSprite->SetSpriteOpacity( 1 );
			CreateHelpButton();
			CreateResumeButton();
			this->removeChild( this->getChildByName( "ExitButton" ) );
		}
		// Otherwise, unpause
		else
		{
			pcSprite->RemoveFromParent();
			this->removeChild( this->getChildByName( "HelpButton"	) );
			this->removeChild( this->getChildByName( "ResumeButton" ) );
			this->removeChild( this->getChildByName( "BackButton" ) );
			cocos2d::Director::getInstance()->resume();			
		}
	}

	else
	{
		// If the help menu bool is false, it will resume it's original iteration of going to next level
		RequestNextLevel();
	}
}

void CManicLayer::CB_OnHelpButton( Ref* pSender )
{
	CGCObjSprite* p_cgc_obj_sprite = EasySprite::Create( "Loose/help_ingame_menu.plist", 4.0f, 1.0f, 1.0f, CC_V2( (m_sizeVisible.width / 2 + m_pointOrigin.x), (m_sizeVisible.height / 2 + m_pointOrigin.y) ), this , "HelpScreen");
	cocos2d::Menu* pMenu;
	MenuItemImage* pItemBack = MenuItemImage::create( "Loose/back_button_normal.png", "back/help_button_pressed.png", CC_CALLBACK_1( CManicLayer::CB_OnBackButton, this, p_cgc_obj_sprite ) );
	pItemBack->setPosition( Vec2( m_pointOrigin.x + (m_sizeVisible.width * 0.11f), m_pointOrigin.y + (m_sizeVisible.height * 0.12f) ) );
	pMenu = Menu::create( pItemBack, nullptr );
	pMenu->setPosition( Vec2::ZERO );
	pMenu->setName( "BackButton" );
	this->removeChild( this->getChildByName( "HelpButton" ) );
	this->removeChild( this->getChildByName( "ResumeButton" ) );

	this->addChild( pMenu, 4 );
}

void CManicLayer::CB_OnResumeButton( Ref* pSender )
{
	this->removeChild( this->getChildByName( "HelpButton" ) );
	this->removeChild( this->getChildByName( "ResumeButton" ) );
	CreateExitButton();
	cocos2d::Director::getInstance()->resume();
}

void CManicLayer::CB_OnBackButton(Ref* pSender, CGCObjSprite* pcSprite)
{
	this->removeChild( this->getChildByName( "BackButton" ) );
	CreateHelpButton();
	CreateResumeButton();
	pcSprite->RemoveFromParent();	  
}

void CManicLayer::CreateExitButton()
{
	const char* pszButtonPath;

	if (m_bUseHelpMenuIngame)
	{
		pszButtonPath = "Loose/help_ingame_button.png";
	}
	else
	{
		pszButtonPath = "Buttons/Exit/ExitButton_01.png";
	}

	MenuItemImage* pItemExitGame = MenuItemImage::create(pszButtonPath,pszButtonPath,CC_CALLBACK_1( CManicLayer::CB_OnGameExitButton, this ) );
	pItemExitGame->setPosition( Vec2( 1900.f, 1060.f ) );

	Menu* pMenu = Menu::create( pItemExitGame, nullptr );
	pMenu->setName( "ExitButton" );
	pMenu->setPosition( Vec2::ZERO );
	this->addChild( pMenu, 5 );
}

void CManicLayer::CreateHelpButton()
{
	cocos2d::Menu* m_pMenu;
	MenuItemImage* pItemHelpScene = MenuItemImage::create( "Loose/help_button_normal.png", "Loose/help_button_pressed.png", CC_CALLBACK_1( CManicLayer::CB_OnHelpButton, this ) );
	pItemHelpScene->setPosition( Vec2( m_pointOrigin.x + (m_sizeVisible.width * 0.5f), m_pointOrigin.y + (m_sizeVisible.height * 0.58f) ) );
	m_pMenu = Menu::create( pItemHelpScene, nullptr );
	m_pMenu->setPosition( Vec2::ZERO );
	m_pMenu->setName( "HelpButton" );
	this->addChild( m_pMenu, 7 );
}

void CManicLayer::CreateResumeButton()
{
	cocos2d::Menu* m_pMenu;
	MenuItemImage* pItemResume = MenuItemImage::create( "Loose/resume_button_normal.png", "Loose/resume_button_pressed.png", CC_CALLBACK_1( CManicLayer::CB_OnResumeButton, this ) );
	pItemResume->setPosition( Vec2( m_pointOrigin.x + (m_sizeVisible.width * 0.5f), m_pointOrigin.y + (m_sizeVisible.height * 0.48f) ) );
	m_pMenu = Menu::create( pItemResume, nullptr );
	m_pMenu->setPosition( Vec2::ZERO );
	m_pMenu->setName( "ResumeButton" );
	this->addChild( m_pMenu, 7 );
}




void CManicLayer::Init()
{
	m_pcHUD = new CHUD( *this, m_pointOrigin, m_sizeVisible );
	m_pcAirManager = new CAirManager( m_pointOrigin, m_sizeVisible );
	m_pcAirManager->Init( *this );

	m_pcGameManager->SetCHUD( m_pcHUD );
	m_pcGameManager->SetCPlayer( &GetPlayer() );
	m_pcGameManager->SetCAirManager( m_pcAirManager );
	m_pcAirManager ->SetGameManager( m_pcGameManager );
	
	if ( m_pcGameManager->GetDoOnce() )
	{
		m_pcGameManager->ResetValues();		
		m_pcGameManager->SetDoOnce( false );
	}

	m_pcGameManager->SetLevelRequirements( m_sLevelCreationParameters.sLevelValues );
	m_pcGameManager->InitCHUD( m_sLevelCreationParameters.szLevelName );

	VInitializeBackground();
}