

#ifndef _AIRMANAGER_H_
#include "AirManager.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "../GameInstance/CGameInstance.h"
#include "../Layers/CManicLayer.h"

USING_NS_CC;


// constructor 
CAirManager::CAirManager( cocos2d::Point pOrigin, cocos2d::Size visibleSize )
	: m_eAirState( EAirState::EAS_HasAirLeft )
	, m_pglOwnerGameLayer		( nullptr )
	, m_pdDirector				( nullptr )
	, m_pcGCSprAirBar			( nullptr )
	, m_plAirLabel				( nullptr )
	, m_pcGCSprAirVignette		( nullptr )
	, m_fMaxAir						( 108.0f )
	, m_fRemainingAirAmount			( m_fMaxAir )
	, m_fConsumedAirAmount			( 0.0f )
	, m_fReduceAirByAmountPerFrame	( 0.0f )
	, m_iRemainingAirPercentage		( 0.0f )
	, m_iConsumedAirPercentage		( 0.0f )
	, m_v2AirLabelPos			( 0.0f, 0.0f )
	, m_bInitialized			( false )
	, m_pOrigin				( pOrigin )
	, m_visibleSize			( visibleSize )

{}

CAirManager::~CAirManager()
{
	// leave no dangling pointers
	m_pglOwnerGameLayer = nullptr;

	m_pdDirector = nullptr;

	// empty variables
	m_fMaxAir = 0.f;
	m_fRemainingAirAmount = 0.f;
	m_fConsumedAirAmount = 0.f;
	m_fReduceAirByAmountPerFrame = 0.f;
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
}

//CAirManager::CAirManager(class IGCGameLayer* pglOwnerGamerLayer, cocos2d::Size* pv2VisibleSize, cocos2d::Point* ppOrigin) : CGCObject(GetGCTypeIDOf(CAirManager))
//{
//	// add checks for pv2VisibleSize and ppOrigin too
//	
//	m_pglOwnerGameLayer = pglOwnerGamerLayer;
//	if(nullptr != m_pglOwnerGameLayer)	// only initialize if a pointer to a gamelayer was provided to the constructor
//	{
//		// initialize member variables
//		// 
//		// the air lasts for 108 seconds
//		m_fMaxAir = 108.f;
//		
//		// remaining air amount should always start with max amount
//		m_fRemainingAirAmount = m_fMaxAir;
//
//		// consumed air should always start at 0 - because the player hasn't consumed any air yet during the construction phase
//		m_fConsumedAirAmount = 0.f;
//		
//		// returns a value from 0-1 therefore it has to be multiplied by 100 to get a range from 0-100 for the percentage
//		m_iRemainingAirPercentage = (m_fRemainingAirAmount / m_fMaxAir) * 100;
//
//		// initialize and store the active instance of the director
//		m_pdDirector = cocos2d::Director::getInstance();
//
//
//		// initialize and add Air Vignette Sprite to the screen
//		const char* pszPlist_AirVignette = "TexturePacker/Air/AirVignette.plist";
//		const char* pszSpr_AirVignette = "AirVignette";
//		{
//			// create new Sprite Object for Air Bar
//			m_pcGCSprAirVignette = new CGCObjSprite();
//			if (nullptr != m_pcGCSprAirVignette)
//			{
//				m_pcGCSprAirVignette->CreateSprite(pszPlist_AirVignette);
//				m_pcGCSprAirVignette->SetResetPosition(Vec2(((*pv2VisibleSize).width / 2), ((*pv2VisibleSize).height / 2)));
//				m_pcGCSprAirVignette->SetParent(IGCGameLayer::ActiveInstance());
//				m_pcGCSprAirVignette->SetSpriteScale(1.f, 1.f);
//				m_pcGCSprAirVignette->SetSpriteOpacity(0);
//				m_pcGCSprAirVignette->SetSpriteGlobalZOrder(2.f);
//			}
//		}
//
//		// initialize and add Air Bar Sprite to the screen
//		const char* pszPlist_Air = "TexturePacker/Air/Air.plist";
//		const char* pszSpr_AirBar = "AirBar";
//		{
//			// create new Sprite Object for Air Bar
//			m_pcGCSprAirBar = new CGCObjSprite();
//			if (nullptr != m_pcGCSprAirBar)
//			{
//				m_pcGCSprAirBar->CreateSprite(pszPlist_Air);
//				m_pcGCSprAirBar->SetResetPosition(Vec2((*ppOrigin).x + 120.f, ((*ppOrigin).y + (*pv2VisibleSize).height) - 24));
//				m_pcGCSprAirBar->SetParent(IGCGameLayer::ActiveInstance());
//				m_pcGCSprAirBar->SetSpriteScale(1.f, 1.f);
//				m_pcGCSprAirBar->SetSpriteGlobalZOrder(3.f);
//			}
//		}
//
//
//		// create and initialize label for air and the percentage
//		m_plAirLabel = Label::createWithTTF("AIR " + std::to_string(m_iRemainingAirPercentage) + "%", "fonts/arial.ttf", 24);
//
//		m_v2AirLabelPos = Vec2((*ppOrigin).x + 60.f, ((*ppOrigin).y + (*pv2VisibleSize).height) - 24);
//		// position pAirLabel in the top left corner
//		m_plAirLabel->setPosition(m_v2AirLabelPos);
//		m_plAirLabel->setGlobalZOrder(3.f);
//		// add the pAirLabel as a child to this layer
//		pglOwnerGamerLayer->addChild(m_plAirLabel, 2);
//	}
//}


void CAirManager::Reset()
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

void CAirManager::Update( float fTimeStep )
{
	UpdateAirTimer();
	UpdateAirUIElements();
}



void CAirManager::Init( class CManicLayer& rglOwnerGameLayer )
{
	//if( !m_bInitialized )
	{
		// add checks for pv2VisibleSize and ppOrigin too

		m_pglOwnerGameLayer = &rglOwnerGameLayer;
		if( nullptr != m_pglOwnerGameLayer )	// only initialize if a pointer to a game layer was provided to the constructor
		{
			// initialize member variables
			// 
			// the air lasts for 108 seconds
			m_fMaxAir = 108.f;

			// remaining air amount should always start with max amount
			m_fRemainingAirAmount = m_fMaxAir;

			// consumed air should always start at 0 - because the player hasn't consumed any air yet during the construction phase
			m_fConsumedAirAmount = 0.f;

			// returns a value from 0-1 therefore it has to be multiplied by 100 to get a range from 0-100 for the percentage
			m_iRemainingAirPercentage = ( m_fRemainingAirAmount / m_fMaxAir ) * 100;

			// initialize and store the active instance of the director
			m_pdDirector = cocos2d::Director::getInstance();


			// initialize and add Air Vignette Sprite to the screen
			const char* pszPlist_AirVignette = "TexturePacker/Air/AirVignette.plist";
			const char* pszSpr_AirVignette = "AirVignette";
			{
				// create new Sprite Object for Air Bar
				if( m_pcGCSprAirVignette == nullptr )
				{
					m_pcGCSprAirVignette = new CGCObjSprite();
					
				}

				if( nullptr != m_pcGCSprAirVignette )
				{
					if( m_pcGCSprAirVignette->GetSprite() == nullptr )
					{
						m_pcGCSprAirVignette->CreateSprite( pszPlist_AirVignette );
					}

					m_pcGCSprAirVignette->SetSpriteScale( 1.f, 1.f );
					m_pcGCSprAirVignette->SetSpriteGlobalZOrder( 2.f );
					m_pcGCSprAirVignette->SetResetPosition( Vec2( ( ( m_visibleSize ).width / 2 ), ( ( m_visibleSize ).height / 2 ) ) );
					m_pcGCSprAirVignette->GetSprite()->setPosition( m_pcGCSprAirVignette->GetResetPosition() );
					m_pcGCSprAirVignette->SetParent( &rglOwnerGameLayer );
					m_pcGCSprAirVignette->SetSpriteOpacity( 0 );
				}
			}

			// initialize and add Air Bar Sprite to the screen
			const char* pszPlist_Air = "TexturePacker/Air/Air.plist";
			const char* pszSpr_AirBar = "AirBar";
			{
				// create new Sprite Object for Air Bar
				if( m_pcGCSprAirBar == nullptr )
				{
					m_pcGCSprAirBar = new CGCObjSprite();
				}

				if( nullptr != m_pcGCSprAirBar )
				{
					if( m_pcGCSprAirBar->GetSprite() == nullptr )
					{
						m_pcGCSprAirBar->CreateSprite( pszPlist_Air );
					}

					m_pcGCSprAirBar->SetSpriteGlobalZOrder( 3.f );
					m_pcGCSprAirBar->SetResetPosition( Vec2( ( m_pOrigin ).x + 120.f, ( ( m_pOrigin ).y + ( m_visibleSize ).height ) - 24 ) );
					m_pcGCSprAirBar->GetSprite()->setPosition( m_pcGCSprAirBar->GetResetPosition() );
					m_pcGCSprAirBar->SetParent( &rglOwnerGameLayer );
					m_pcGCSprAirBar->SetSpriteScale( 1.f, 1.f );
				}
			}


			// create and initialize label for air and the percentage
			if( m_plAirLabel == nullptr )
			{
				m_plAirLabel = Label::createWithTTF( "AIR " + std::to_string( m_iRemainingAirPercentage ) + "%", "fonts/arial.ttf", 24 );
				m_plAirLabel->setGlobalZOrder( 3.f );
			}

			m_v2AirLabelPos = Vec2( ( m_pOrigin ).x + 60.f, ( ( m_pOrigin ).y + ( m_visibleSize ).height ) - 24 );
			// position pAirLabel in the top left corner
			m_plAirLabel->setPosition( m_v2AirLabelPos );
			// add the pAirLabel as a child to this layer
			rglOwnerGameLayer.addChild( m_plAirLabel, 2 );
		}
		m_bInitialized = true;
	}
}


bool CAirManager::GetHasInitialized()
{
	return m_bInitialized;
}

bool CAirManager::UpdateAirTimer()
{
	if( m_fRemainingAirAmount <= 0.0f ) // if no air left return false
	{
		CGameInstance::getInstance()->OutOfAir();
		return false;
	}

	m_fReduceAirByAmountPerFrame = 1 / m_pdDirector->getFrameRate();	   // using framerate for division allows 

	m_fRemainingAirAmount -= m_fReduceAirByAmountPerFrame;			       // reduce air by 1 second / framerate every frame - example: for 60 frames the equation would
																		   // like this: 1 / 60 and the result would be 0.0166666666666667f

	m_fConsumedAirAmount += m_fReduceAirByAmountPerFrame;	     		   //
	m_iConsumedAirPercentage = ( m_fConsumedAirAmount / m_fMaxAir ) * 100;

	m_iRemainingAirPercentage = ( m_fRemainingAirAmount / m_fMaxAir ) * 100; // Air Remaining / Max Air returns a value from 0-1 therefore it has to be multiplied by 100 to
																		   // get a range from 0-100 for the percentage

	return true;
}

void CAirManager::UpdateAirUIElements()
{
	if( nullptr != m_pglOwnerGameLayer && nullptr != m_pcGCSprAirBar->GetSprite() && m_pcGCSprAirVignette->GetSprite()  !=nullptr && m_plAirLabel != nullptr )
	{
		// remove already existing label
		m_pglOwnerGameLayer->removeChild( m_plAirLabel );
		// add a label displaying Air
		// create and initialize label for air
		m_plAirLabel = Label::createWithTTF( "AIR " + std::to_string( m_iRemainingAirPercentage ) + "%", "fonts/arial.ttf", 24 );
		// position pAirLabel in the top left corner
		m_plAirLabel->setPosition( m_v2AirLabelPos );
		m_plAirLabel->setGlobalZOrder( 3.f );
		// add the pAirLabel as a child to this layer
		m_pglOwnerGameLayer->addChild( m_plAirLabel, 2 );

		if( 0 <= m_pcGCSprAirBar->GetSpriteScale().x )
		{
			float fNewWidth = m_fRemainingAirAmount / m_fMaxAir; // store this in a variable so that it doesn't have to be calculated multiple times
			m_pcGCSprAirBar->SetSpriteScale( fNewWidth, 1.f );
		}

		if( 0 <= m_pcGCSprAirVignette->GetSpriteOpacity() && nullptr != m_pcGCSprAirVignette )
		{
			int iNewOpacity = ( m_fConsumedAirAmount / m_fMaxAir ) * 250;
			m_pcGCSprAirVignette->SetSpriteOpacity( iNewOpacity );
		}
	}
}