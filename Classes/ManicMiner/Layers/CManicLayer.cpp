#include <algorithm>
#include <stdlib.h>

#include "CManicLayer.h"

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"



#include "AppDelegate.h"

#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "GamerCamp/GCObject/GCObjectManager.h"

#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "ManicMiner/Hazards/GCObjHazard.h"
#include "ManicMiner/Collectible/CCollectible.h"
#include "ManicMiner/CollectiblesGroup/CCollectiblesGroup.h"
#include "ManicMiner/Player/CPlayer.h"
#include "MenuScene.h"
#include "ManicMiner/GameInstance/CGameInstance.h"
#include "ManicMiner/LevelManager/CLevelManager.h"
//#include "GamerCamp/GameSpecific/Platforms/GCObjPlatform.h"
#include "ManicMiner/Helpers/Helpers.h"

// Include different platform types for collision checks
#include "ManicMiner/Platforms/CPlatform.h"
#include "ManicMiner/Platforms/CBrickPlatform.h"
#include "ManicMiner/Platforms/CCrumblingPlatform.h"
#include "ManicMiner/Platforms/CGroundPlatform.h"
#include "ManicMiner/Platforms/CMovingPlatform.h"
#include "ManicMiner/Platforms/CRegularPlatform.h"


USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// this just demos how simple it is to turn on/off logging on a define....
#define ENABLE_COLLISION_TEST_LOGGING

#if defined (ENABLE_COLLISION_TEST_LOGGING)

#define COLLISIONTESTLOG( str )		CCLOG( str )

#else

#define COLLISIONTESTLOG( str )		/*nothing*/

#endif


///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
CManicLayer::CManicLayer()
	: IGCGameLayer( GetGCTypeIDOf( CManicLayer ) )
	, m_pcLevelManager( nullptr )
	, m_eGameState( EGameState::EGS_Looting )
	, m_bWasResetRequested( false )
	, m_bWasNextLevelRequested( false )
	, m_pcGCSprBackGround( nullptr )
	, m_pcPlayer( nullptr )
	, m_eCollectibleTypeRequired( ECollectibleTypeRequired::Collectible )
	, m_iNumCollectiblesNeeded( 4 )
{

}

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
CManicLayer::~CManicLayer()
{}

//////////////////////////////////////////////////////////////////////////
// in order to guarantee the actions this layer expects we need to 
// initialise the keyboard action map every time onEnter is called - this
// fixes the case where an IGCGameLayer with a different mapping was
// pushed over this one on Director's scene stack
//////////////////////////////////////////////////////////////////////////
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

void CManicLayer::onExit()
{
	IGCGameLayer::onExit();
}

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

	// add "CGCGameLayerPlatformer" splash screen"
	const char* pszPlist_background = "TexturePacker/Backgrounds/Placeholder/background.plist";
	{
		m_pcGCSprBackGround = new CGCObjSprite();
		m_pcGCSprBackGround->CreateSprite( pszPlist_background );
		m_pcGCSprBackGround->SetResetPosition( Vec2( visibleSize.width / 2, ( visibleSize.height / 2 ) - 60.f ) );
		m_pcGCSprBackGround->SetParent( IGCGameLayer::ActiveInstance() );
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
	// add player
	///////////////////////////////////////////////////////////////////////////

	// starting position
	const Vec2 v2MarioStartPos = v2ScreenCentre_Pixels;

	// create player object
	m_pcPlayer = new CPlayer( *this, v2MarioStartPos );


	GetCollisionManager().AddCollisionHandler( [&] ( CPlayer& rcPlayer, CPlatform& rcPlatform, const b2Contact& rcContact ) -> void
		{
			PlatformCollided( rcPlayer, rcPlatform, rcContact );
		} );


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







	//Label* pcScoreLabel = Label::create();

}// void CGCGameLayerPlatformer::VOnCreate() { ...

//////////////////////////////////////////////////////////////////////////
// on update
//////////////////////////////////////////////////////////////////////////
//virtual 
void CManicLayer::VOnUpdate( f32 fTimeStep )
{
	IGCGameLayer::VOnUpdate( fTimeStep );

	// this shows how to iterate and respond to the box2d collision info
	HandleCollisions();

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

	IGCGameLayer::VOnDestroy();
}
/// Sensor bits will go here
///////////////////////////////////////////////////////////////////////////////
// begin contact
// insert any logic that relies on detecting the first frame where a 
// contact exists
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CManicLayer::BeginContact( b2Contact* pB2Contact )
{

	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();

	const b2Body* pBodyA = pFixtureA->GetBody();
	const b2Body* pBodyB = pFixtureB->GetBody();

	CGCObjSpritePhysics* pGcSprPhysA = (CGCObjSpritePhysics*)pBodyA->GetUserData();
	// if( this is not a GC object )
	if (pGcSprPhysA == nullptr)
	{
		return;
	}

	CGCObjSpritePhysics* pGcSprPhysB = (CGCObjSpritePhysics*)pBodyB->GetUserData();
	// if( this is not a GC object )
	if (pGcSprPhysB == nullptr)
	{
		return;
	}





	// Henrique
	// CHECK FOR PLATORM AND PLAYER COLLISION
	if (pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID())
	{
		if (((pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ))
			&& (pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlatform )))
			|| ((pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlatform ))
			&& (pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlayer ))))
		{

			// Get pointer to platform
			CPlatform* pPlatform = nullptr;

			// If BodyA is player, BodyB must be platform
			if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )
			{
				pPlatform = static_cast< CPlatform* >( pGcSprPhysB );
			}
			// Else, vice versa
			else
			{
				pPlatform = static_cast< CPlatform* >( pGcSprPhysA );
			}



			// Check if this BeginContact is for feet + platform surface ( sensors only )
			
			if( pFixtureA->IsSensor() && pFixtureB->IsSensor() )
			{
				CCLOG( "Foot goes toot" );

				// Activate this platform's collision
				pPlatform->SetCollisionEnabled( true );


				// Increment sensor count for the player
				m_pcPlayer->SensorContactEvent( true );
			}

			// If not 2 sensors
			// Check if this BeginContact is for character + platform ( no sensors included )
			else if( !( pFixtureA->IsSensor() ) && !( pFixtureB->IsSensor() ) )
			{
				if( pPlatform->GetCollisionEnabled() )
				{


					// Set the platform as a trigger for hard contact events
					pPlatform->SetTriggersHardContactEvent( true );

					// Increment hard contact count for the player
					m_pcPlayer->HardContactEvent( true );


					// Check Platform Type
					switch( pPlatform->GetPlatformType () )
					{
						////////////////////////////////////////////////////////////////////////////////
						// MOVING																	////
						////////////////////////////////////////////////////////////////////////////////
					case EPT_Moving:
						{
							m_pcPlayer->LandedOnConveyorBelt( EPlayerDirection::EPD_Left ); // needs to access platform direction
						}	
						break;
						////////////////////////////////////////////////////////////////////////////////
						// CRUMBLING																////
						////////////////////////////////////////////////////////////////////////////////
					case EPT_Crumbling:
						{
							// Start Crumbling
						auto pCrumblingPlatform = static_cast< CCrumblingPlatform* > ( pPlatform );

						if( pCrumblingPlatform != nullptr )
						{
							pCrumblingPlatform->InitiateCrumbling( 1.0f );
						}

							// Set player as grounded
							m_pcPlayer->LandedOnWalkablePlatform();
						}
						break;
						////////////////////////////////////////////////////////////////////////////////
						// BRICK																	////
						////////////////////////////////////////////////////////////////////////////////
					case EPT_Brick:
						{
						// If in mid air and sensor contacts == 0
							if( ( m_pcPlayer->IsInMidAir() ) && ( m_pcPlayer->GetSensorContactCount() == 0 ) || ( m_pcPlayer->GetIsOnConveyorBelt() ) )
							{
								// Player Bumped onto brick
								m_pcPlayer->BumpedWithBricks();
							}
							else
							{
								// Set player as grounded
								m_pcPlayer->LandedOnWalkablePlatform();
							}
						}
					break;
					////////////////////////////////////////////////////////////////////////////////
					// DEFAULT																	////
					////////////////////////////////////////////////////////////////////////////////
					default:
						{
							// Set player as grounded
							m_pcPlayer->LandedOnWalkablePlatform();
						}
						break;
					}
					
				}
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// end contact
// insert any logic that relies on detecting the last frame where a 
// contact exists
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CManicLayer::EndContact( b2Contact* pB2Contact )
{
	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();

	const b2Body* pBodyA = pFixtureA->GetBody();
	const b2Body* pBodyB = pFixtureB->GetBody();

	CGCObjSpritePhysics* pGcSprPhysA = ( CGCObjSpritePhysics* )pBodyA->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysA == nullptr )
	{
		return;
	}

	CGCObjSpritePhysics* pGcSprPhysB = ( CGCObjSpritePhysics* )pBodyB->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysB == nullptr )
	{
		return;
	}




	// CHECK FOR PLATORM AND PLAYER COLLISION
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) ) )
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) ) ) )
		{

			// Get pointer to platform
			CPlatform* pPlatform = nullptr;

			// If BodyA is player, BodyB must be platform
			if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )
			{
				pPlatform = static_cast< CPlatform* >( pGcSprPhysB );
			}
			// Else, vice versa
			else
			{
				pPlatform = static_cast< CPlatform* >( pGcSprPhysA );
			}



			// Check if this EndContact is for feet + platform surface sensors only
			if( pFixtureA->IsSensor() && pFixtureB->IsSensor() )
			{
				CCLOG( "Foot goes untoot" );

				// If this platform is not a CBrickPlatform
				if( pPlatform->GetPlatformType() != EPlatformType::EPT_Brick )
				{
					// Deactivate this platform's collision
					pPlatform->SetCollisionEnabled( false );
				}

				// Decrement sensor contact count
				m_pcPlayer->SensorContactEvent( false );
			}

			// If not 2 sensors
			// Check if this EndContact is for character + platform ( no sensors included )
			else if( !( pFixtureA->IsSensor() ) && !( pFixtureB->IsSensor() ) )
			{
				if( pPlatform->GetTriggersHardContactEvent() )
				{

					// Set the platform as no trigger for hard contact events
					pPlatform->SetTriggersHardContactEvent( false );

					// Decrement hard contact count
					m_pcPlayer->HardContactEvent( false );

					// If feet are no longer touching any ground surface
					if( !m_pcPlayer->GetHardContactCount() && m_pcPlayer->GetIsGrounded() )
					{
						m_pcPlayer->LeftGround();
						CCLOG( "LeftGround()" );
					}
				}
			}
		}
	}
}
// If not jumping > then falling
// Collisions off



///////////////////////////////////////////////////////////////////////////////
// pre solve
// insert any logic that needs to be done before a contact is resolved
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CManicLayer::PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold )
{
	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();

	const b2Body* pBodyA = pFixtureA->GetBody();
	const b2Body* pBodyB = pFixtureB->GetBody();

	CGCObjSpritePhysics* pGcSprPhysA = ( CGCObjSpritePhysics* )pBodyA->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysA == nullptr )
	{
		return;
	}

	CGCObjSpritePhysics* pGcSprPhysB = ( CGCObjSpritePhysics* )pBodyB->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysB == nullptr )
	{
		return;
	}

	// Henrique
	// CHECK FOR PLATORM AND PLAYER COLLISION
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) ) )
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlatform ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) ) ) )
		{
			// Check if this BeginContact is not running with any sensor triggers
			if( !( pFixtureA->IsSensor() ) && !( pFixtureB->IsSensor() ) )
			{
				// We must check if this platform is supposed to have collision or not

				// Get pointer to platform
				CPlatform* pPlatform = nullptr;

				// If BodyA is player, BodyB must be platform
				if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CPlayer ) )
				{
					pPlatform = static_cast< CPlatform* >( pGcSprPhysB );
				}
				// Else, vice versa
				else
				{
					pPlatform = static_cast< CPlatform* >( pGcSprPhysA );
				}

				// Set contact collision accordingly
				pB2Contact->SetEnabled( pPlatform->GetCollisionEnabled() );
			}

		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// post solve
// insert any logic that needs to be done after a contact is resolved
// e.g. check the types and double the impulse
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CManicLayer::PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse )
{}

///////////////////////////////////////////////////////////////////////////////
// this function can only look at the collisions that happened (and which 
// have potentially been resolved) in the previous physics step
// 
// N.B. it is totally possible to get a callback after collisions have been 
// detected, but before they're resolved so you can choose to ignore them - 
// e.g. for gamplay reasons like jumping up through a platform
// 
///////////////////////////////////////////////////////////////////////////////
///
void CManicLayer::HandleCollisions()
{
	//// check for collisions
	//b2Body* pBodyToDestroy = nullptr;
	//for( const b2Contact* pB2Contact = IGCGameLayer::ActiveInstance()->B2dGetWorld()->GetContactList();
	//	nullptr != pB2Contact;
	//	pB2Contact = pB2Contact->GetNext() )
	//{
	//	const b2Fixture* pFixtureA = CGCObjSpritePhysics::FromB2DContactGetFixture_A( pB2Contact );
	//	const b2Fixture* pFixtureB = CGCObjSpritePhysics::FromB2DContactGetFixture_B( pB2Contact );

	//	// return if either physics body has null user data
	//	CGCObjSpritePhysics* pGcSprPhysA = CGCObjSpritePhysics::FromB2DFixtureGetSpritePhysics( pFixtureA );
	//	if( !pGcSprPhysA )
	//	{
	//		return;
	//	}

	//	CGCObjSpritePhysics* pGcSprPhysB = CGCObjSpritePhysics::FromB2DFixtureGetSpritePhysics( pFixtureB );
	//	if( !pGcSprPhysB )
	//	{
	//		return;
	//	}

	//	// check for user data - this is defined in physics editor as the 'Id' text
	//	// in the text box immediately below the 'Is Sensor?' checkbox
	//	// 
	//	// Mario has a fixture that is a sensor with id 'bottom_left' 
	//	// and this is what we're checking for :)
	//	const std::string*  pstrCheckMe = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureA );
	//	bool				bNameMatches = ( 0 == pstrCheckMe->compare( "bottom_left" ) );
	//	bool				bIsASensor = pFixtureA->IsSensor();

	//	if( pstrCheckMe && bNameMatches && bIsASensor )
	//	{
	//		int i = 0;
	//		++i;
	//	}

	//	pstrCheckMe = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureB );
	//	if( pstrCheckMe
	//		&& ( 0 == pstrCheckMe->compare( "bottom_left" ) )
	//		&& pFixtureB->IsSensor() )
	//	{
	//		int i = 0;
	//		++i;
	//	}
	//}
}

// GetPhysicsBody()->GetFixtureList()->IsSensor();

////////////////////////////////////////////////////////////////////////////
// Object Specific Collision Handles ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


// Player + Enemy
void CManicLayer::PlayerCollidedEnemy( CPlayer& rcPlayer, CGCObjEnemy& rcEnemy, const b2Contact& rcContact )
{
	if( rcContact.IsTouching() )
	{
		OnDeath();
	}
}

// Player + Hazard
void CManicLayer::PlayerCollidedHazard(CPlayer& rcPlayer, CGCObjHazard& rcHazard, const b2Contact& rcContact)
{
	if( rcContact.IsTouching() )
	{
		OnDeath();
	}
}



void CManicLayer::EnemyCollidedPlatform( CGCObjEnemy& rcEnemy, const b2Contact& rcContact )
{
	if( rcContact.IsTouching() )
	{
		rcEnemy.BounceEnemyDirection();
	}
}


// Player + Collectible
void CManicLayer::ItemCollected( CCollectible& rcCollectible, CPlayer& rcPlayer, const b2Contact& rcContact )
{
	if( rcContact.IsTouching() )
	{
		rcCollectible.InteractEvent();
	}
}

void CManicLayer::PlatformCollided( CPlayer& rcPlayer, CPlatform& rcPlatform, const b2Contact& rcContact )
{
	const bool isColliding = rcContact.IsTouching();
	if( isColliding )
	{

		
		//else
		//{

		//	rcPlayer.SetCanBeControlled( true );

		//}

		//Vec2 const rcPlatformPosition = rcPlatform.GetSpritePosition();
		//Vec2 const rcPlayerPosition = rcPlayer.GetSpritePosition();
		//if( rcPlayerPosition.y > rcPlatformPosition.y )
		//{

		//}
		//else
		//{
		//	if( m_eGameState == EGameState::EGS_Escaping )
		//	{
		//		OnEscaped();
		//	}
		//}
	}
}

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

void CManicLayer::OnFinishedLooting()
{
	m_eGameState = EGameState::EGS_Escaping;

	//GetCollisionManager().AddCollisionHandler( [&] ( CPlayer& rcPlayer, CGCObjPlatform& rPlatform, const b2Contact& rcContact ) -> void
	//	{
	//		if( m_eGameState == EGameState::EGS_Escaping )
	//		{
	//			OnEscaped();
	//		}
	//	} );
}

void CManicLayer::OnEscaped()
{
	m_eGameState = EGameState::EGS_Victory;
	// Run Time animation and points and stuff
	// when that ends, call this line:
	RequestNextLevel();
}

void CManicLayer::OutOfLives()
{
	m_pcLevelManager->GoToMainMenu();
}

CPlayer& CManicLayer::GetPlayer() const
{
	return *m_pcPlayer;
}

CLevelManager& CManicLayer::GetLevelManager() const
{
	return *m_pcLevelManager;
}

void CManicLayer::SetLevelManager( CLevelManager& rcLevelManager )
{
	m_pcLevelManager = &rcLevelManager;
}

void CManicLayer::CB_OnGameExitButton( Ref* pSender )
{
	// add code to release anything that needs to be released before exiting the game
	RequestNextLevel();
}

void CManicLayer::ResetLevel()
{
	ResetRequestWasHandled();
	VOnReset();
}

void CManicLayer::RequestNextLevel()
{
	m_bWasNextLevelRequested = true;
}
