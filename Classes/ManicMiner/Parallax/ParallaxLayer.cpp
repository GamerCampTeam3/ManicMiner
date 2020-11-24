#include "ParallaxLayer.h"

#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "2d/CCScene.h"

CParallaxLayer::CParallaxLayer()
	: m_pcSprite	( nullptr )
	, m_psData		( nullptr )
{
}

CParallaxLayer::~CParallaxLayer()
{
	if( m_pcSprite )
	{
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

	m_pcSprite = new CGCObjSprite();
	m_pcSprite->CreateSprite( m_psData->pszPlist_image );
	m_pcSprite->SetResetPosition( cocos2d::Vec2::ZERO );
	rcScene.addChild( GetSprite(), 0 );

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