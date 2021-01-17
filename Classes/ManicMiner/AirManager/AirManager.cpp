

#ifndef _AIRMANAGER_H_
#include "AirManager.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "../GameInstance/CGameInstance.h"
#include "../Layers/CMLCentralCavern.h"

#include "ManicMiner/GameManager/CGameManager.h"
#include "ManicMiner/Layers/CManicLayer.h"

USING_NS_CC;


// constructor 
CAirManager::CAirManager(cocos2d::Point pOrigin, cocos2d::Size visibleSize )
	: CGCObject(GetGCTypeIDOf(CAirManager))
	, m_eAirState( EAirState::HasAirLeft )
	, m_eAirDrainedState(EAirDrainedState::AirDrained)
	, m_pglOwnerGameLayer			( nullptr )
	, m_pdDirector					( nullptr )
	, m_pcGCSprAirBar				( nullptr )
	, m_plAirLabel					( nullptr )
	, m_pcGCSprAirVignette			( nullptr )
	, m_pcLBAirBar					( nullptr )
	, m_fMaxAir						( 108.0f )
	, m_fRemainingAirAmount			( m_fMaxAir )
	, m_fConsumedAirAmount			( 0.0f )
	, m_fReduceAirByAmountPerFrame	( 0.0f )
	, m_fRemainingAirPercentage		( 0.0f )
	, m_iRemainingAirPercentage		( 0 )
	, m_iConsumedAirPercentage		( 0.0f )
	, m_v2AirLabelPos				( 0.0f, 0.0f )
	, m_bInitialized				( false )
	, m_pOrigin						( pOrigin )
	, m_visibleSize					( visibleSize )
	, m_fDrainAirMultiplier			( 1.0f )
	, m_bDrainComplete				( false )
	, m_pcGameManager				( nullptr )
{}

CAirManager::~CAirManager()
{
	if (nullptr != m_pglOwnerGameLayer && nullptr != m_pdDirector)
	{
		m_pglOwnerGameLayer = nullptr;
		m_pdDirector = nullptr;
	}


	
	// empty variables
	m_fMaxAir = 0.f;
	m_fRemainingAirAmount = 0.f;
	m_fConsumedAirAmount = 0.f;
	m_fReduceAirByAmountPerFrame = 0.f;
	m_fRemainingAirPercentage = 0;
	m_iRemainingAirPercentage = 0;
	m_iConsumedAirPercentage = 0;
	m_v2AirLabelPos = cocos2d::Vec2( 0.f, 0.f );

	if ( m_pcGCSprAirBar )
	{
		m_pcGCSprAirBar->GetSprite()->release();
		delete m_pcGCSprAirBar;
		m_pcGCSprAirBar = nullptr;
	}	

	if ( m_plAirLabel )
	{
		m_plAirLabel->release();
		delete m_plAirLabel;
		m_plAirLabel = nullptr;
	}

	if( m_pcGCSprAirVignette )
	{
		m_pcGCSprAirVignette->GetSprite()->release();
		delete m_pcGCSprAirVignette;
		m_pcGCSprAirVignette = nullptr;
	}

	if( m_pcLBAirBar )
	{
		m_pcLBAirBar->release();
		delete m_pcLBAirBar;
		m_pcLBAirBar = nullptr;
	}

	if( nullptr != m_pcGameManager )
	{
		m_pcGameManager = nullptr;
	}
}

void CAirManager::VOnReset()
{
	m_fRemainingAirAmount = m_fMaxAir;
	m_fConsumedAirAmount = 0.f;
	m_pcGCSprAirBar->SetSpriteScale( 1.f, 1.f );
}

void CAirManager::LeavingLevel( CManicLayer& rNewManicLayer )
{
	if( m_pcGCSprAirBar->GetSprite() != nullptr )
	{
		m_pcGCSprAirBar->GetSprite()->retain();
		m_pcGCSprAirVignette->RemoveFromParent();
	}
	if( m_pcGCSprAirVignette->GetSprite() != nullptr )
	{
		m_pcGCSprAirVignette->GetSprite()->retain();
		m_pcGCSprAirVignette->RemoveFromParent();
	}
	
	// We know that all the assets were automatically released
	// Which means the Air Bar Sprite, Vignette Sprite and the Air Label no longer exist in memory
	// Therefore, AirLabel needs to be set to nullptr
	if( m_plAirLabel != nullptr )
	{
		m_plAirLabel = nullptr;
	}
}

void CAirManager::VOnUpdate( float fTimeStep )
{
	CGCObject::VOnUpdate(fTimeStep);

	// Timer Calculations (Drain every frame) and UI Elements need to be updated every tick 
	UpdateAirTimer();
	UpdateAirUIElements();
}



void CAirManager::Init(  CManicLayer& rglOwnerGameLayer)
{
	m_pglOwnerGameLayer =& rglOwnerGameLayer;
	if( nullptr != m_pglOwnerGameLayer )	// only initialize if a pointer to a game layer was provided to the constructor
	{
		// initialize member variables
		// 
		// timer runs out after 108 seconds
		m_fMaxAir = 108.f;

		// timer should always start with the max amount of seconds left
		m_fRemainingAirAmount = m_fMaxAir;

		// consumed air should always start at 0 - because the player hasn't consumed any air yet when the AirManager is initialized
		m_fConsumedAirAmount = 0.f;

			//
		// returns a value from 0-1 therefore it has to be multiplied by 100 to get a range from 0-100 for the percentage
		m_fRemainingAirPercentage = ( m_fRemainingAirAmount * 100) / m_fMaxAir;
		m_iRemainingAirPercentage = ( m_fRemainingAirAmount * 100) / m_fMaxAir;

		// initialize and store the active instance of the director
		m_pdDirector = cocos2d::Director::getInstance();


		// initialize and add Air Vignette Sprite to the screen
		const char* pszPlist_AirVignette = "TexturePacker/Air/AirVignette.plist";
		const char* pszSpr_AirVignette = "AirVignette";
		{
			// create new Sprite Object for Air Bar, if one doesn't already exist
			if( m_pcGCSprAirVignette == nullptr )
			{
				m_pcGCSprAirVignette = new CGCObjSprite();
				
			}

			if( nullptr != m_pcGCSprAirVignette )
			{
				if( m_pcGCSprAirVignette->GetSprite() == nullptr )
				{
					// load in and create sprite based on the image inside the plist 
					m_pcGCSprAirVignette->CreateSprite( pszPlist_AirVignette );
				}

				// Initialize AirVignette Sprite and set it's opacity to 0,
				// as it's opacity will go from 0 to 1, based on the remaining
				// amount of air left
				m_pcGCSprAirVignette->SetSpriteScale( 1.f, 1.f );
				m_pcGCSprAirVignette->SetSpriteGlobalZOrder( 2.f );
				m_pcGCSprAirVignette->SetSpriteScale(2.f, 2.f);
				m_pcGCSprAirVignette->SetResetPosition( Vec2( ( ( m_visibleSize ).width / 2 ), ( ( m_visibleSize ).height / 2 ) ) );
				m_pcGCSprAirVignette->GetSprite()->setPosition( m_pcGCSprAirVignette->GetResetPosition() );
				m_pcGCSprAirVignette->SetParent( &rglOwnerGameLayer );
				m_pcGCSprAirVignette->SetSpriteOpacity( 0 );
			}
		}
		
		// initialize and add Air Bar Sprite to the screen
		const char* pszPlist_Air = "TexturePacker/Air/AirBarBackground.plist";
		const char* pszSpr_AirBar = "AirBar";
		
			// create new Sprite Object for Background Element for Air Bar, if one doesn't already exist
			if( m_pcGCSprAirBar == nullptr )
			{
				m_pcGCSprAirBar = new CGCObjSprite();
			}

			if( nullptr != m_pcGCSprAirBar )
			{
				if( m_pcGCSprAirBar->GetSprite() == nullptr )
				{
					// load in and create sprite based on the image inside the plist 
					m_pcGCSprAirBar->CreateSprite( pszPlist_Air );
				}

				// Initial Parameters 
				m_pcGCSprAirBar->SetSpriteGlobalZOrder( 2.f );
				// same Position as the AirBar, because it's supposed to
				// serve as the background element for the actual "Loading Bar"
				m_pcGCSprAirBar->SetResetPosition( Vec2( ( m_pOrigin ).x + 100.f, ( ( m_pOrigin ).y + ( m_visibleSize ).height ) - 60 ) );
				m_pcGCSprAirBar->GetSprite()->setPosition( m_pcGCSprAirBar->GetResetPosition() );
				m_pcGCSprAirBar->SetParent( &rglOwnerGameLayer );
				m_pcGCSprAirBar->SetSpriteScale( 1.f, 1.f );
			}
		
		// create new loading bar object, if one doesn't already exist
		if (nullptr == m_pcLBAirBar)
		{
			m_pcLBAirBar = new ui::LoadingBar();
		}

		if (nullptr != m_pcLBAirBar)
		{
			// initialize the loading bar parameters and create a texture for it
			// based on the image in the specified path 
			m_pcLBAirBar = ui::LoadingBar::create("ui/AirBar/ui_bar_full.png");

			// Since the LoadingBar only has two directions, Left and Right,
			// we need to chose one of them, Left in my case, and then
			// rotate it so that the direction is bottom
			m_pcLBAirBar->setDirection(ui::LoadingBar::Direction::LEFT);
			
			// since the Air Bar is supposed to drain from top to bottom,
			// it needs to be rotated by 90 Degrees (Counterclockwise)
			m_pcLBAirBar->setRotation(-90.f);
			
			// will start at 100% and slowly go down to 0%
			m_pcLBAirBar->setPercent(100.f);
			m_pcLBAirBar->setPosition(Vec2((m_pOrigin).x + 100.f, ((m_pOrigin).y + (m_visibleSize).height) - 60 ));
			rglOwnerGameLayer.addChild(m_pcLBAirBar, 3);
			m_pcLBAirBar->setGlobalZOrder(3.f);
		}

	}
	m_bInitialized = true;
}


bool CAirManager::GetHasInitialized()
{
	return m_bInitialized;
}

void CAirManager::DrainAir()
{
	m_fDrainAirMultiplier = 40.f;
	m_eAirDrainedState = EAirDrainedState::LevelCompleted;
}

void CAirManager::SunlightDrainAir(bool bShouldDrainAir)
{
	if(bShouldDrainAir)
	{
		m_fDrainAirMultiplier = 5.f;
	}
	else
	{
		m_fDrainAirMultiplier = 1.f;
	}
}

bool CAirManager::UpdateAirTimer()
{
	
	bool bHasAirLeft = true;
	if( m_fRemainingAirAmount <= 0.0f )													// if no air left return false
	{	// replace with if statement  - cheaper
		switch (m_eAirDrainedState)
		{
		case EAirDrainedState::AirDrained :
			m_pglOwnerGameLayer->OnDeath();
			break;
		case EAirDrainedState::LevelCompleted :
			break;
		}
		bHasAirLeft = false;
		m_bDrainComplete = true;
	}

	// using framerate for division allows 
	m_fReduceAirByAmountPerFrame = (1.f / m_pdDirector->getFrameRate()) * m_fDrainAirMultiplier;

	// reduce air by 1 second / framerate every frame - example: for 60 frames the equation would
	// like this: 1 / 60 and the result would be 0.0166666666666667f
	m_fRemainingAirAmount -= m_fReduceAirByAmountPerFrame;

	m_fConsumedAirAmount += m_fReduceAirByAmountPerFrame;	     						
	m_iConsumedAirPercentage = ( m_fConsumedAirAmount * 100.f ) / m_fMaxAir ;

	// Air Remaining / Max Air returns a value from 0-1 therefore it has to be multiplied by 100 to
	// get a range from 0-100 for the percentage
	m_fRemainingAirPercentage = ( m_fRemainingAirAmount * 100.f) / m_fMaxAir;

	// floors the float and stores it as an integer
	m_iRemainingAirPercentage = m_fRemainingAirPercentage;

	//--------------------------------------------------------------------------------------------------------------
	// Bibs Implementation
	// Not really a great way of doing this, since AirManager doesn't necessarily need to know about the CGameManager
	// But it does work, ideally I will replicate later the per frame thing Umeer has done.
	if (m_pcGameManager->GetCanDrainToScore())
	{
		m_pcGameManager->DrainToScore();
	}
	//--------------------------------------------------------------------------------------------------------------
	
	return bHasAirLeft;
}

void CAirManager::SetGameManager(CGameManager* pcGameManager)
{
	m_pcGameManager = pcGameManager;
}


void CAirManager::UpdateAirUIElements()
{
	if( nullptr != m_pglOwnerGameLayer && nullptr != m_pcGCSprAirBar->GetSprite() && m_pcGCSprAirVignette->GetSprite()  !=nullptr )
	{
		if(0.0f <= m_pcLBAirBar->getPercent())
		{
			// Update Percentage of the LoadingBar to
			// visually represent how much air is left
			m_pcLBAirBar->setPercent(m_fRemainingAirPercentage);
		}

		// Bib Edit: Disable vignette fade if leaving the level
		if (m_pglOwnerGameLayer->GetGameState() != EGameState::Escaping)
		{
			if (0 <= m_pcGCSprAirVignette->GetSpriteOpacity() && nullptr != m_pcGCSprAirVignette)
			{
				// Update the Opacity of the Vignette effect based on how much air has been consumed
				// Opacity ranges from 0-250, therefore the value, in range of 0 to 1, we get from
				// m_fConsumedAirAmount / m_fMaxAir needs to be multiplied by 250 to map it to 0-250.
				int iNewOpacity = (m_fConsumedAirAmount * 250 / m_fMaxAir);
				m_pcGCSprAirVignette->SetSpriteOpacity( iNewOpacity );
			}
		}
	}
}