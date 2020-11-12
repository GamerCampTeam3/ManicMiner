// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Team 3																								//
// -------------------------------------------------------------------------------------------------------------------- //

#include "CManicLayer.h"

#include "AppDelegate.h"

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/Win32Input/GCKeyboardManager.h"

#include "ManicMiner/Collectible/CCollectible.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"
#include "ManicMiner/Doors/CDoor.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "ManicMiner/Hazards/GCObjHazard.h"
#include "ManicMiner/Helpers/Helpers.h"
#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/LevelManager/CLevelManager.h"

// Include different platform types for collision checks
#include "ManicMiner/Platforms/CBrickPlatform.h"
#include "ManicMiner/Platforms/CCrumblingPlatform.h"
#include "ManicMiner/Platforms/CGroundPlatform.h"
#include "ManicMiner/Platforms/CMovingPlatform.h"
#include "ManicMiner/Platforms/CPlatform.h"
#include "ManicMiner/Platforms/CRegularPlatform.h"
#include "ManicMiner/Switch/CSwitch.h"

#include "ManicMiner/Player/CPlayer.h"

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
	, m_pczBackGround				( nullptr )
	, m_sLevelValues				( ECollectibleRequirements::Collectible, 5 )
	, m_pcLevelManager				( nullptr )
	, m_eGameState					( EGameState::Looting )
	, m_bWasResetRequested			( false )
	, m_bWasNextLevelRequested		( false )
	, m_pcGCSprBackGround			( nullptr )
	, m_pcPlayer					( nullptr )
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
	///////////////////////////////////////////////////////////////////////////
	// cache some useful values 
	///////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// cache some useful values 
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();


	///////////////////////////////////////////////////////////////////////////
	// default object group
	///////////////////////////////////////////////////////////////////////////

	// create the default object group
	IGCGameLayer::VOnCreate();

	///////////////////////////////////////////////////////////////////////////
	/// Exit Button

	MenuItemImage* pItemExitGame = MenuItemImage::create(
		"Buttons/Exit/ExitButton_01.png",
		"Buttons/Exit/ExitButton_01.png",
		CC_CALLBACK_1( CManicLayer::CB_OnGameExitButton, this ) );

	pItemExitGame->setPosition( Vec2( 1900.f, 1060.f ) );

	Menu* pMenu = Menu::create( pItemExitGame, nullptr );
	pMenu->setPosition( Vec2::ZERO );
	this->addChild( pMenu, 4 );
	///
	///////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////
	// custom object groups
	//
	// N.B. Cannot do this in CGCObjectGroup internally on construction, 
	// because ObjectGroupRegister calls a virtual function 
	// in the CGCObjectManager interface to check the 
	// types of objects that the group handles
	///////////////////////////////////////////////////////////////////////////

	// Adds the background
	// Make sure to set the value of m_pczBackGround before this is ran otherwise it will never have a texture.
	if (m_pczBackGround != nullptr )
	{
		InitializeBackground( visibleSize );
	}



	///////////////////////////////////////////////////////////////////////////
	// set up physics 
	///////////////////////////////////////////////////////////////////////////

	// set "self" as contact listener
	B2dGetWorld()->SetContactListener( this );

	// load the physics shapes from the plist created with PhysicsEditor
	B2dLoadShapesFromPlist( "PhysicsEditor/GameShapes.plist" );

	///////////////////////////////////////////////////////////////////////////
	// stop Mario from leaving the screen
	///////////////////////////////////////////////////////////////////////////

	// get window size
	//Size s = Director::getInstance()->getWinSize();

	// PTM_RATIO
	f32 PTM_RATIO = IGCGAMELAYER_B2D_PIXELS_PER_METER;

	b2Vec2	b2v2ScreenCentre_Pixels( ( origin.x + ( visibleSize.width * 0.5f ) ), ( origin.y + ( visibleSize.height * 0.5f ) ) );
	Vec2	v2ScreenCentre_Pixels( ( origin.x + ( visibleSize.width * 0.5f ) ), ( origin.y + ( visibleSize.height * 0.5f ) ) );
	Vec2	v2ScreenCentre_Offset( ( origin.x + ( visibleSize.width * 0.5f ) ), ( origin.y + ( visibleSize.height * 0.4f ) ) );



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
	groundBox.SetAsBox( ( ( visibleSize.width * 0.5f ) / PTM_RATIO ), 0.5f, b2Vec2( 0.0f, -( ( visibleSize.height * 0.5f ) / PTM_RATIO ) ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// top
	groundBox.SetAsBox( ( ( visibleSize.width * 0.5f ) / PTM_RATIO ), 0.5f, b2Vec2( 0.0f, ( ( visibleSize.height * 0.5f ) / PTM_RATIO ) ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// left
	groundBox.SetAsBox( 0.5f, ( ( visibleSize.height * 0.5f ) / PTM_RATIO ), b2Vec2( -( ( visibleSize.width * 0.5f ) / PTM_RATIO ), 0.0f ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// right
	groundBox.SetAsBox( 0.5f, ( ( visibleSize.height * 0.5f ) / PTM_RATIO ), b2Vec2( ( ( visibleSize.width * 0.5f ) / PTM_RATIO ), 0.0f ), 0.0f );
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
	
	// load level data from Ogmo Editor

	// read the oel file for level 0
	m_cLevelLoader.LoadLevelFile( FileUtils::getInstance()->fullPathForFilename( std::string( "OgmoEditor/AirBush.oel" ) ).c_str() );
	m_cLevelLoader.CreateObjects( CGCFactory_ObjSpritePhysics::GetFactory() );

	// note: we have now created all the items, platforms, & invaders specified in the level file




	///////////////////////////////////////////////////////////////////////////
	// add player
	///////////////////////////////////////////////////////////////////////////

	// starting position
	const Vec2 v2PlayerStartPos = v2ScreenCentre_Pixels;
	// create player object
	m_pcPlayer = new CPlayer( *this, v2PlayerStartPos );



	GetCollisionManager().AddCollisionHandler( [&] ( CPlayer& rcPlayer, CCollectible& rcCollectible, const b2Contact& rcContact ) -> void
		{
			ItemCollected( rcCollectible, rcPlayer, rcContact );
		} );

	GetCollisionManager().AddCollisionHandler( [&] ( CPlayer& rcPlayer, CGCObjEnemy& rcEnemy, const b2Contact& rcContact ) -> void
		{
			PlayerCollidedEnemy( rcPlayer, rcEnemy, rcContact );
		} );

	GetCollisionManager().AddCollisionHandler( [&] (CPlayer& rcPlayer, CGCObjHazard& rcHazard, const b2Contact& rcContact) -> void
		{
			PlayerCollidedHazard(rcPlayer, rcHazard, rcContact);
		});


	GetCollisionManager().AddCollisionHandler( [&] (CPlayer& rcPlayer, CDoor& rcDoor, const b2Contact& rcContact) -> void
		{
			PlayerCollidedDoor( rcPlayer, rcDoor, rcContact );
		});

}

void CManicLayer::InitializeBackground(const cocos2d::Size& rSize)
{
	m_pcGCSprBackGround = new CGCObjSprite();
	m_pcGCSprBackGround->CreateSprite( m_pczBackGround );
	m_pcGCSprBackGround->SetResetPosition( Vec2( rSize.width / 2, (rSize.height / 2) - 60.f ) );
	m_pcGCSprBackGround->SetParent( IGCGameLayer::ActiveInstance() );
}


//////////////////////////////////////////////////////////////////////////
// on update
//////////////////////////////////////////////////////////////////////////
//virtual 
void CManicLayer::VOnUpdate( f32 fTimeStep )
{
	IGCGameLayer::VOnUpdate( fTimeStep );

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
	///
	//safeDelete( m_pcPlayer );
	//safeDelete( m_pcGCSprBackGround );
	
	// Clean up the level
	m_cLevelLoader.DestroyObjects();
	
	IGCGameLayer::VOnDestroy();
}



void CManicLayer::VOnReset( void )
{
	IGCGameLayer::VOnReset();
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
			CPlatform* pPlatform = nullptr;																				//
																														//
			// If BodyA is player, BodyB must be platform																//
			if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )												//
			{																											//
				pPlatform = static_cast< CPlatform* >( pGcSprPhysB );													//
			}																											//
			// Else, vice versa																							//
			else																										//
			{																											//
				pPlatform = static_cast< CPlatform* >( pGcSprPhysA );													//
			}																											//
																														//
																														//
																														//
			// Check if this BeginContact is for feet + platform surface ( sensors only )								//
			if( pFixtureA->IsSensor() && pFixtureB->IsSensor() )														//
			{																											//

				// GET ID NAMESSSSS WOOP took exactly 1 hour to find out
				const std::string* pszSensorIdA = GB2ShapeCache::getFixtureIdText( pFixtureA );
				auto pszSensorIdB = GB2ShapeCache::getFixtureIdText( pFixtureB );

				// Check if this platform was already overlapping in terms of hard contact
				bool bShouldStartHardContact = ( pPlatform->GetIsInContact() && !pPlatform->GetCollisionEnabled() ) && m_pcPlayer->GetHardContactCount();

				// Activate this platform's collision																	//
				pPlatform->SetCollisionEnabled( true );																	//
																														//
																														//
				// Increment sensor count for the player																//
				m_pcPlayer->SensorContactEvent( true );																	//

				pPlatform->SetIsSensorOverlapped( true );
				
				if( bShouldStartHardContact )
				{
					CCLOG( "Forcing a ghosted hard contact start, due to sensor contact now but hard fixtures were already overlapping." );
					PlayerBeganContactWithPlatform( *pPlatform );
				}

			}																											//
																														//
			// If not 2 sensors																							//
			// Check if this BeginContact is for character + platform ( no sensors included )							//
			else if( !( pFixtureA->IsSensor() ) && !( pFixtureB->IsSensor() ) )											//
			{																											//
				PlayerBeganContactWithPlatform( *pPlatform );
			}																											//
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
			CPlatform* pPlatform = nullptr;																				//
																														//
			// If BodyA is player, BodyB must be platform																//
			if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )												//
			{																											//
				pPlatform = static_cast< CPlatform* >( pGcSprPhysB );													//
			}																											//
			// Else, vice versa																							//
			else																										//
			{																											//
				pPlatform = static_cast< CPlatform* >( pGcSprPhysA );													//
			}																											//
																														//
																														//
																														//
			// Check if this EndContact is for feet + platform surface sensors only										//
			if( pFixtureA->IsSensor() && pFixtureB->IsSensor() )														//
			{																											//																														//
				// If this platform is not a CBrickPlatform																//
				if( pPlatform->GetPlatformType() != EPlatformType::Brick )												//
				{																										//
					// Deactivate this platform's collision																//
					pPlatform->SetCollisionEnabled( pPlatform->GetTriggersHardContactEvent() );							//
				}																										//
																														//
				// Decrement sensor contact count																		//
				m_pcPlayer->SensorContactEvent( false );																//

				pPlatform->SetIsSensorOverlapped( false );
			}																											//
																														//
			// If not 2 sensors																							//
			// Check if this EndContact is for character + platform ( no sensors included )								//
			else if( !( pFixtureA->IsSensor() ) && !( pFixtureB->IsSensor() ) )											//
			{																											//
				if( pPlatform->GetTriggersHardContactEvent() )															//
				{																										//
					// Set the platform as no trigger for hard contact events											//
					pPlatform->SetTriggersHardContactEvent( false );													//
																														//
					// Decrement hard contact count																		//
					m_pcPlayer->HardContactEvent( false );																//
																														//
					// If feet are no longer touching any ground surface												//
					if( !m_pcPlayer->GetHardContactCount() && m_pcPlayer->GetIsGrounded() )								//
					{																									//
						m_pcPlayer->LeftGround();																		//
					}																									//
					// If this was a Crumbling platform, stop crumbling													//
					switch( pPlatform->GetPlatformType() )																//
					{																									//
					case EPlatformType::Crumbling:																		//
						auto pCrumblingPlatform = static_cast< CCrumblingPlatform* > ( pPlatform );						//
																														//
						if( pCrumblingPlatform != nullptr )																//
						{																								//
							pCrumblingPlatform->StopCrumbling();														//
						}																								//
						break;																							//
					}																									//
					// If leaving this contact, and sensors aren't overlapping anymore as well
					if ( !pPlatform->GetIsSensorOverlapped() && pPlatform->GetPlatformType() != EPlatformType::Brick )
					{
						// Disable platform collision
						pPlatform->SetCollisionEnabled( false );
					}
				}																										//
				else
				{
#ifdef PLAYER_DEBUG_VIRTUAL_CONTACTS
					CCLOG( "ENDED SOLID CONTACT WITH A PLATFORM THAT HAS COLLISION OFF" );
#endif
				}
				pPlatform->SetIsInContact( false );
			}																											//
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
				CPlatform* pPlatform = nullptr;																			//
																														//
				// If BodyA is player, BodyB must be platform															//
				if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )											//
				{																										//
					pPlatform = static_cast< CPlatform* >( pGcSprPhysB );												//
				}																										//
				// Else, vice versa																						//
				else																									//
				{																										//
					pPlatform = static_cast< CPlatform* >( pGcSprPhysA );												//
				}																										//
																														//
				// Set contact collision accordingly																	//
				pB2Contact->SetEnabled( pPlatform->GetCollisionEnabled() );												//
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
		rcEnemy.BounceEnemyDirection();																					//
	}																													//
}																														//
																														//
																														//
void CManicLayer::PlayerCollidedEnemy( CPlayer& rcPlayer, CGCObjEnemy& rcEnemy, const b2Contact& rcContact )			//
{																														//
	if( rcContact.IsTouching() )																						//
	{																													//
		OnDeath();																										//
	}																													//
}																														//
																														//
																														//
void CManicLayer::PlayerCollidedHazard( CPlayer& rcPlayer, CGCObjHazard& rcHazard, const b2Contact& rcContact )			//
{																														//
	if( rcContact.IsTouching() )																						//
	{																													//
		OnDeath();
		m_pcGameManager->UpdateLives();//
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
		m_pcGameManager->CCollectibleInteractEvent();																	//
	}																													//
}																														//
																														//
void CManicLayer::SwitchInteracted(CSwitch& rcSwitch, CPlayer& rcPlayer, const b2Contact& rcContact)					//
{																														//
	if (rcContact.IsTouching())																							//
	{																													//
		rcSwitch.InteractEvent();																						//
		m_pcGameManager->CSwitchInteractEvent();																		//
	}																													//
}																														//
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
		EventKeyboard::KeyCode::KEY_SPACE,			// EPA_Fire	
	};

	u32 uSizeOfActionArray = ( sizeof( aeKeyCodesForActions ) / sizeof( cocos2d::EventKeyboard::KeyCode ) );

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager( uSizeOfActionArray, aeKeyCodesForActions );
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
	m_pcLevelManager->GoToMainMenu();
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
			if( ( !m_pcPlayer->GetIsGrounded() ) && ( m_pcPlayer->GetSensorContactCount() == 0 )			//
				|| ( m_pcPlayer->GetIsOnConveyorBelt() ) )													//
			{																								//
				// Player Bumped onto brick																	//
				m_pcPlayer->BumpedWithBricks();																//
			}																								//
			// Else, is in mid-air? If so, land on platform													//
			else if( !m_pcPlayer->GetIsGrounded() )															//
			{																								//
				// Set player as grounded																	//
				m_pcPlayer->LandedOnWalkablePlatform();														//
			}																								//
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
	RequestNextLevel();
}