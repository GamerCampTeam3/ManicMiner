// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira & Umeer Rama																		//
// -------------------------------------------------------------------------------------------------------------------- //

#include "ParallaxLayer.h"

#include "2d/CCScene.h"
#include "GamerCamp/GCCocosInterface/GCObjSprite.h"

CParallaxLayer::CParallaxLayer()
	: m_pcSprite	( nullptr )
	, m_psData		( nullptr )
{
}

CParallaxLayer::~CParallaxLayer()
{
	// Clean up layer sprite
	if( m_pcSprite ) 
	{
		m_pcSprite->DestroySprite();

		delete m_pcSprite;
		m_pcSprite = nullptr;
	}

	if( m_psData )
	{
		delete m_psData;
		m_psData = nullptr;
	}
}

void CParallaxLayer::Init( cocos2d::Scene& rcScene, const SParallaxLayerData& rsData )
{
	// Assign new data ( SParallaxLayerData )
	m_psData = new SParallaxLayerData( rsData );

	// Create new Sprite
	m_pcSprite = new CGCObjSprite();

	if( m_psData->kpszPlist_image )
	{
		m_pcSprite->CreateSprite( m_psData->kpszPlist_image );
		m_pcSprite->SetResetPosition( cocos2d::Vec2::ZERO );
		// Add to scene in correct Z-Order
		rcScene.addChild( GetSprite(), m_psData->kiZOrder );
	}
}

cocos2d::Sprite* CParallaxLayer::GetSprite() const
{
	if( m_pcSprite != nullptr )
	{
		return m_pcSprite->GetSprite();
	}
	else
	{
		return nullptr;
		CCLOG( "ERROR: CParallaxLayer::GetSprite() CALLED BEFORE CParallaxLayer::Init(), GOT NULLPTR FOR SPRITE" );
	}
}

const SParallaxLayerData* CParallaxLayer::GetData() const
{
	return m_psData;
}
