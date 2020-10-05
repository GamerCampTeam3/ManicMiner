#include "CTestLayer.h"

#include "cocos2d.h"

#include "GamerCamp/GameSpecific/Invaders/GCObjGroupInvader.h"

#include "GamerCamp/GameSpecific/Player/GCObjPlayer.h"
#include "GamerCamp/GameSpecific/Invaders/GCObjInvader.h"
#include "GamerCamp/GameSpecific/Invaders/GCObjGroupInvader.h"
#include "GamerCamp/GameSpecific/Items/GCObjItem.h"
#include "GamerCamp/GameSpecific/Platforms/GCObjPlatform.h"


//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CTestLayer::CTestLayer()
	: CGCGameLayerPlatformer()
	, m_eGameState( EGameState::EGS_Looting )
	, m_iLivesLeft( 3 )
	, m_iCollected( 0 )
{}


//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
CTestLayer::~CTestLayer()
{}

void CTestLayer::VOnCreate()
{
	CGCGameLayerPlatformer::VOnCreate();
	//////////////////////////////////////////////////////////////////////
	// Add specific collision handles
	//////////////////////////////////////////////////////////////////////
	GetCollisionManager().AddCollisionHandler( [&] ( CGCObjPlayer& rcPlayer, CGCObjInvader& rcInvader, const b2Contact& rcContact ) -> void
		{
			PlayerCollidedInvader( rcPlayer, rcInvader, rcContact );
		} );

	GetCollisionManager().AddCollisionHandler( [&] ( CGCObjPlayer& rcPlayer, CGCObjItem& rItem, const b2Contact& rcContact ) -> void
		{
			ItemCollected( rItem, rcContact );
		} );
}

void CTestLayer::VOnUpdate( f32 fTimeStep )
{
	CGCGameLayerPlatformer::VOnUpdate( fTimeStep );
}

void CTestLayer::OnFinishedLooting()
{
	SetGameState( EGameState::EGS_Escaping );
	GetCollisionManager().AddCollisionHandler( [&] ( CGCObjPlayer& rcPlayer, CGCObjPlatform& rItem, const b2Contact& rcContact ) -> void
		{
			if( m_eGameState == EGameState::EGS_Escaping )
			{
				OnEscaped();
			}
		} );
}

void CTestLayer::OnEscaped()
{
	SetGameState( EGameState::EGS_Victory );
	cocos2d::Director::getInstance()->replaceScene( cocos2d::TransitionRotoZoom::create( 1.0f, TGCGameLayerSceneCreator< CTestLayer >::CreateScene() ) );
}

void CTestLayer::OnPlayerDeath( CGCObjPlayer& rPlayer )
{
	m_iLivesLeft--;
}

void CTestLayer::OutOfLives()
{
	CGCObjectManager::ObjectKill( m_pcGCOPlayer );
}

////////////////////////////////////////////////////////////////////////////
// Object Specific Collision Handles ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Player + Enemy
void CTestLayer::PlayerCollidedInvader( CGCObjPlayer& rPlayer, CGCObjInvader& rInvader, const b2Contact& rcContact )
{
	OnPlayerDeath( rPlayer );
	if( m_iLivesLeft )
	{
		RequestReset();
	}
	else
	{
		OutOfLives();
	}
}

// Player + Collectible
void CTestLayer::ItemCollected( CGCObjItem& rItem, const b2Contact& rcContact )
{
	CGCObjectManager::ObjectKill( &rItem );
	m_iCollected++;

	if( m_eGameState == EGameState::EGS_Looting && m_iCollected > 3 )
	{
		OnFinishedLooting();
	}
}




////////////////////////////////////////////////////////////////////////////
// Getters	////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
EGameState CTestLayer::GetGameState()
{
	return m_eGameState;
}

////////////////////////////////////////////////////////////////////////////
// Setters	////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void CTestLayer::SetGameState( EGameState eGameState )
{
	m_eGameState = eGameState;
}
