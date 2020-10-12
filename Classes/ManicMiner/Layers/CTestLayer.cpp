#include "CTestLayer.h"

#include <algorithm>
#include <stdlib.h> 

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"



#include "AppDelegate.h"


#include "ManicMiner/Player/CPlayer.h"
#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/Invaders/GCObjGroupInvader.h"
#include "GamerCamp/GameSpecific/Invaders/GCObjInvader.h"
#include "GamerCamp/GameSpecific/Items/GCObjItem.h" 
#include "GamerCamp/GameSpecific/Items/GCObjGroupItem.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjGroupPlatform.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjPlatform.h" 
#include "GamerCamp/GameSpecific/Player/GCObjGroupProjectilePlayer.h"
#include "GamerCamp/GameSpecific/Player/GCObjPlayer.h"

#include "MenuScene.h"
#include "../GameInstance/CGameInstance.h"





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
CTestLayer::CTestLayer()
	: IGCGameLayer( GetGCTypeIDOf( CTestLayer ) )
	, m_pcGCGroupItem( nullptr )
	, m_pcGCGroupInvader( nullptr )
	, m_pcGCGroupProjectilePlayer( nullptr )
	, m_pcGCSprBackGround( nullptr )
	, m_pcGCOPlayer( nullptr )
	, m_bResetWasRequested( false )
{}


//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
CTestLayer::~CTestLayer()
{}


//////////////////////////////////////////////////////////////////////////
// in order to guarantee the actions this layer expects we need to 
// initialise the keyboard action map every time onEnter is called - this
// fixes the case where an IGCGameLayer with a different mapping was
// pushed over this one on Director's scene stack
//////////////////////////////////////////////////////////////////////////
void CTestLayer::onEnter()
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

//////////////////////////////////////////////////////////////////////////
// on create
//////////////////////////////////////////////////////////////////////////
//virtual
void CTestLayer::VOnCreate()
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
	// custom object groups
	//
	// N.B. Cannot do this in CGCObjectGroup internally on construction, 
	// because ObjectGroupRegister calls a virtual function 
	// in the CGCObjectManager interface to check the 
	// types of objects that the group handles
	///////////////////////////////////////////////////////////////////////////

	// create and register the object group for the platform objects
	m_pcGCGroupPlatform = new CGCObjGroupPlatform();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupPlatform );

	// create and register the object group for the item objects
	m_pcGCGroupItem = new CGCObjGroupItem();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupItem );

	// create and register the object group for the invader objects
	m_pcGCGroupInvader = new CGCObjGroupInvader();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupInvader );

	// create and register the object group for the player projectile objects
	m_pcGCGroupProjectilePlayer = new CGCObjGroupProjectilePlayer();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupProjectilePlayer );


	///////////////////////////////////////////////////////////////////////////
	// add menu
	///////////////////////////////////////////////////////////////////////////

	// add a "close" icon to exit the progress. it's an autorelease object
	MenuItemImage* pResetItem
		= MenuItemImage::create(	"Loose/CloseNormal.png",
									"Loose/CloseSelected.png",
									CC_CALLBACK_1( CTestLayer::Callback_OnResetButton, this ) );

	pResetItem->setPosition( Vec2( ( ( visibleSize.width - ( pResetItem->getContentSize().width * 0.5f ) ) + origin.x ),
		( ( ( pResetItem->getContentSize().height * 0.5f ) + origin.y ) ) ) );

	MenuItemImage* pQuitItem
		= MenuItemImage::create(	"Loose/CloseNormal.png",
									"Loose/CloseSelected.png",
									CC_CALLBACK_1( CTestLayer::Callback_OnQuitButton, this ) );

	pQuitItem->setPosition( Vec2( ( ( visibleSize.width - ( pQuitItem->getContentSize().width * 0.5f ) ) + origin.x ),
		( ( visibleSize.height - ( pQuitItem->getContentSize().height * 0.5f ) ) + origin.y ) ) );

	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create( pResetItem, pQuitItem, nullptr );
	pMenu->setPosition( Vec2::ZERO );
	this->addChild( pMenu, 1 );


	////////////////////////////////////////////////////////////////////////////
	// add label
	////////////////////////////////////////////////////////////////////////////

	// create and initialize a label
	Label* pLabel = Label::createWithTTF( "Game - Top button to Quit, bottom button to Reset", "fonts/arial.ttf", 24 );

	// position the label on the center of the screen
	pLabel->setPosition( Vec2( visibleSize.width / 2, visibleSize.height - 50 ) );

	// add the label as a child to this layer
	this->addChild( pLabel, 1 );

	// add "CGCGameLayerPlatformer" splash screen"
	const char* pszPlist_background = "TexturePacker/Backgrounds/Placeholder/background.plist";
	{
		m_pcGCSprBackGround = new CGCObjSprite();
		m_pcGCSprBackGround->CreateSprite( pszPlist_background );
		m_pcGCSprBackGround->SetResetPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2 ) );
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
	Vec2 v2MarioStartPos = v2ScreenCentre_Pixels;

	// create player object
	m_pcGCOPlayer = new CPlayer(v2MarioStartPos);

	///////////////////////////////////////////////////////////////////////////
	// N.B. invaders are added by the invader object group
	///////////////////////////////////////////////////////////////////////////
	m_pcGCGroupInvader->SetFormationOrigin( v2ScreenCentre_Pixels + Vec2( -( visibleSize.width * 0.3f ), ( visibleSize.height * 0.25f ) ) );

	///////////////////////////////////////////////////////////////////////////
	// add platforms & items
	///////////////////////////////////////////////////////////////////////////
	// CGCObjGroupItem & CGCObjGroupPlatform clean up all contained objects during VOnGroupResourceRelease(), so we can be 
	// very laissez faire about creating items & platforms
	const u32 uNumColumns = 3;
	const u32 uNumRows = 4;

	float fColumnSpacing = ( visibleSize.width / ( ( ( float )uNumColumns ) + 1.0f ) );
	float fRowSpacing = ( visibleSize.height / ( ( ( float )uNumRows ) + 1.0f ) );

	float fNextPlatformPos_x = fColumnSpacing;
	float fRowStartPos_y = fRowSpacing;

	for( u32 uColumn = 0; uColumn < uNumColumns; ++uColumn )
	{
		Vec2 v2NextPlatformPos( fNextPlatformPos_x, fRowStartPos_y );

		for( u32 uRow = 0; uRow < uNumRows; ++uRow )
		{
			CGCObjPlatform* pPlatform = new CGCObjPlatform();
			CGCObjItem* pItem = new CGCObjItem();

			pPlatform->SetResetPosition( v2NextPlatformPos );
			pItem->SetResetPosition( v2NextPlatformPos + Vec2( 0.0f, 30.0f ) );

			v2NextPlatformPos.y += fRowSpacing;
		}

		fNextPlatformPos_x += fColumnSpacing;
	}

	//////////////////////////////////////////////////////////////////////
	// Add specific collision handles
	//////////////////////////////////////////////////////////////////////

	GetCollisionManager().AddCollisionHandler( [] ( CPlayer& rcPlayer, CGCObjItem& rcItem, const b2Contact& rcContact ) -> void
		{
			COLLISIONTESTLOG( "(lambda) the player hit an item!" );
		} );
	GetCollisionManager().AddCollisionHandler( [&] (CPlayer& rcPlayer, CGCObjInvader& rcInvader, const b2Contact& rcContact ) -> void
		{
			PlayerCollidedInvader( rcPlayer, rcInvader, rcContact );
		} );

	GetCollisionManager().AddCollisionHandler( [&] (CPlayer& rcPlayer, CGCObjItem& rItem, const b2Contact& rcContact ) -> void
		{
			ItemCollected( rItem, rcContact );
		} );

}// void CGCGameLayerPlatformer::VOnCreate() { ...


//////////////////////////////////////////////////////////////////////////
// on update
//////////////////////////////////////////////////////////////////////////
//virtual 
void CTestLayer::VOnUpdate( f32 fTimeStep )
{
	IGCGameLayer::VOnUpdate( fTimeStep );

	// this shows how to iterate and respond to the box2d collision info
	HandleCollisions();

	if( ResetWasRequested() )
	{
		VOnReset();
		ResetRequestWasHandled();
	}
}


///////////////////////////////////////////////////////////////////////////////
// on destroy
///////////////////////////////////////////////////////////////////////////////
// virtual
void CTestLayer::VOnDestroy()
{
	///////////////////////////////////////////////////////////////////////////
	// clean up anything we allocated in opposite order to creation
	///////////////////////////////////////////////////////////////////////////	
	delete m_pcGCOPlayer;
	m_pcGCOPlayer = nullptr;

	delete m_pcGCSprBackGround;
	m_pcGCSprBackGround = nullptr;

	///////////////////////////////////////////////////////////////////////////
	// N.B. because object groups must register manually, 
	// we also unregister them manually
	///////////////////////////////////////////////////////////////////////////
	CGCObjectManager::ObjectGroupUnRegister( m_pcGCGroupPlatform );
	delete m_pcGCGroupPlatform;
	m_pcGCGroupPlatform = nullptr;

	CGCObjectManager::ObjectGroupUnRegister( m_pcGCGroupProjectilePlayer );
	delete m_pcGCGroupProjectilePlayer;
	m_pcGCGroupProjectilePlayer = nullptr;

	CGCObjectManager::ObjectGroupUnRegister( m_pcGCGroupInvader );
	delete m_pcGCGroupInvader;
	m_pcGCGroupInvader = nullptr;

	CGCObjectManager::ObjectGroupUnRegister( m_pcGCGroupItem );
	delete m_pcGCGroupItem;
	m_pcGCGroupItem = nullptr;

	IGCGameLayer::VOnDestroy();
}


///////////////////////////////////////////////////////////////////////////////
// on quit button
///////////////////////////////////////////////////////////////////////////////
void CTestLayer::Callback_OnQuitButton( Ref* pSender )
{
	ReplaceScene( TransitionRotoZoom::create( 1.0f, CMenuLayer::scene() ) );
}



///////////////////////////////////////////////////////////////////////////////
// on reset button
///////////////////////////////////////////////////////////////////////////////
void CTestLayer::Callback_OnResetButton( Ref* pSender )
{
	RequestReset();
}


///////////////////////////////////////////////////////////////////////////////
// begin contact
// insert any logic that relies on detecting the first frame where a 
// contact exists
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CTestLayer::BeginContact( b2Contact* pB2Contact )
{}


///////////////////////////////////////////////////////////////////////////////
// end contact
// insert any logic that relies on detecting the last frame where a 
// contact exists
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CTestLayer::EndContact( b2Contact* pB2Contact )
{}


///////////////////////////////////////////////////////////////////////////////
// pre solve
// insert any logic that needs to be done before a contact is resolved
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CTestLayer::PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold )
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

	// ignore contact between player projectile and item for collision resolution purposes
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjProjectilePlayer ) )
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjItem ) ) )
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjItem ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjProjectilePlayer ) ) ) )
		{
			// ignore the collision!
			pB2Contact->SetEnabled( false );

			//
			// insert logic relating to this collision here
			//
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// post solve
// insert any logic that needs to be done after a contact is resolved
// e.g. check the types and double the impulse
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CTestLayer::PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse )
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
void CTestLayer::HandleCollisions()
{
	// check for collisions
	b2Body* pBodyToDestroy = nullptr;
	for( const b2Contact* pB2Contact = IGCGameLayer::ActiveInstance()->B2dGetWorld()->GetContactList();
		nullptr != pB2Contact;
		pB2Contact = pB2Contact->GetNext() )
	{
		const b2Fixture* pFixtureA = CGCObjSpritePhysics::FromB2DContactGetFixture_A( pB2Contact );
		const b2Fixture* pFixtureB = CGCObjSpritePhysics::FromB2DContactGetFixture_B( pB2Contact );

		// return if either physics body has null user data
		CGCObjSpritePhysics* pGcSprPhysA = CGCObjSpritePhysics::FromB2DFixtureGetSpritePhysics( pFixtureA );
		if( !pGcSprPhysA )
		{
			return;
		}

		CGCObjSpritePhysics* pGcSprPhysB = CGCObjSpritePhysics::FromB2DFixtureGetSpritePhysics( pFixtureB );
		if( !pGcSprPhysB )
		{
			return;
		}

		// check for user data - this is defined in physics editor as the 'Id' text
		// in the text box immediately below the 'Is Sensor?' checkbox
		// 
		// Mario has a fixture that is a sensor with id 'bottom_left' 
		// and this is what we're checking for :)
		const std::string* pstrCheckMe = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureA );
		bool				bNameMatches = ( 0 == pstrCheckMe->compare( "bottom_left" ) );
		bool				bIsASensor = pFixtureA->IsSensor();

		if( pstrCheckMe && bNameMatches && bIsASensor )
		{
			int i = 0;
			++i;
		}

		pstrCheckMe = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureB );
		if( pstrCheckMe
			&& ( 0 == pstrCheckMe->compare( "bottom_left" ) )
			&& pFixtureB->IsSensor() )
		{
			int i = 0;
			++i;
		}

		// is this collision an invader and a projectile?
		// if so, kill the invader
		if( pB2Contact->IsEnabled()
			&& ( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() ) )
		{
			// returns a valid ptr if instance's EXACT type matches or nullptr if not
			CGCObjInvader* pInvaderA = CGCObject::SafeCastToDerived< CGCObjInvader* >( pGcSprPhysA );
			CGCObjInvader* pInvaderB = CGCObject::SafeCastToDerived< CGCObjInvader* >( pGcSprPhysB );

			// at least one of them is an invader?
			if( pInvaderA || pInvaderB )
			{
				CGCObjInvader* pKillMe = ( pInvaderA ? pInvaderA : pInvaderB );
				GCTypeID		tidNotInvader = ( pInvaderA ? pGcSprPhysB->GetGCTypeID() : pGcSprPhysA->GetGCTypeID() );

				if( GetGCTypeIDOf( CGCObjProjectilePlayer ) == tidNotInvader )
				{
					CGCObjectManager::ObjectKill( pKillMe );
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// Object Specific Collision Handles ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Player + Enemy
void CTestLayer::PlayerCollidedInvader(CPlayer& rPlayer, CGCObjInvader& rInvader, const b2Contact& rcContact )
{
	CGameInstance::getInstance()->OnPlayerDeath( rPlayer );
}

// Player + Collectible
void CTestLayer::ItemCollected( CGCObjItem& rItem, const b2Contact& rcContact )
{
	CGameInstance::getInstance()->OnItemCollected( rItem );
}