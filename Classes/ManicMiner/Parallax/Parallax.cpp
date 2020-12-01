#include "Parallax.h"

#include "2d/CCScene.h"

#include "AnimatedParallaxLayer.h"
#include "ScrollingParallaxLayer.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ParallaxLayer.h"
#include "base/ccMacros.h"

CParallax::CParallax		( const int kiNumScrollingLayers, const int kiNumAnimatedLayers, cocos2d::Scene& rcScene, CPlayer& rcPlayer )
	: m_rcScene				( rcScene				)
	, m_rcPlayer			( rcPlayer				)
	, m_kiNumScrollingLayers( kiNumScrollingLayers	)
	, m_kiNumAnimatedLayers	( kiNumAnimatedLayers	)
	, m_pacScrollingLayers	( nullptr				)
	, m_pacAnimatedLayers	( nullptr				)
	, m_pcPlatformOutlineSprite(nullptr)
	, m_iCurrentLayer		( 0						)
{
	m_pacScrollingLayers	= new CScrollingParallaxLayer[m_kiNumScrollingLayers];
	if( m_kiNumAnimatedLayers )
	{
		m_pacAnimatedLayers		= new CAnimatedParallaxLayer[m_kiNumAnimatedLayers];
	}
}

CParallax::~CParallax()
{
	if( m_pacScrollingLayers != nullptr )
	{
		delete[] m_pacScrollingLayers;
		m_pacScrollingLayers = nullptr;
	}
	if( m_pcPlatformOutlineSprite )
	{
		m_pcPlatformOutlineSprite->DestroySprite();
		delete m_pcPlatformOutlineSprite;
		m_pcPlatformOutlineSprite = nullptr;
	}
	if( m_pacAnimatedLayers != nullptr )
	{
		delete[] m_pacAnimatedLayers;
		m_pacScrollingLayers = nullptr;
	}

}

void CParallax::AddScrollingLayer( const SParallaxLayerData& rsLayerData, const float kfMovementScale )
{
	CCAssert( (m_iCurrentLayer < m_kiNumScrollingLayers ), "Trying to add more layers than allocated to CParallax" );
	CCAssert( rsLayerData.kpszPlist_image != nullptr, "CParallaxLayer pszPlist incorrect path" );

	CScrollingParallaxLayer& rcCurrentLayer = m_pacScrollingLayers[m_iCurrentLayer];

	rcCurrentLayer.Init( m_rcScene, rsLayerData, kfMovementScale, &m_rcPlayer );
	m_iCurrentLayer++;
}

void CParallax::AddAnimatedLayer(const SParallaxLayerData& rsLayerData)
{
	CAnimatedParallaxLayer& rcCurrentLayer = m_pacAnimatedLayers[0];
	rcCurrentLayer.Init(m_rcScene, rsLayerData);
}

void CParallax::AddPlatformOutlines( const char* kpszPlatformOutlinesPlist )
{
	CC_ASSERT( m_pcPlatformOutlineSprite == nullptr );

	const float kfScreenCentreX = 960.0f;
	const float kfScreenCentreY = 540.0f;

	m_pcPlatformOutlineSprite = new CGCObjSprite();
	m_pcPlatformOutlineSprite->CreateSprite( kpszPlatformOutlinesPlist );
	m_pcPlatformOutlineSprite->SetResetPosition( cocos2d::Vec2( kfScreenCentreX, kfScreenCentreY ) );
	m_rcScene.addChild( m_pcPlatformOutlineSprite->GetSprite(), -2 );
}

void CParallax::Update()
{
	for( int iCurrentLayer = 0; iCurrentLayer < m_kiNumScrollingLayers; iCurrentLayer++ )
	{
		m_pacScrollingLayers[ iCurrentLayer ].VUpdate();
	}
	m_pacAnimatedLayers[0].VUpdate();
}

void CParallax::Reset()
{
	for( int iCurrentLayer = 0; iCurrentLayer < m_kiNumScrollingLayers; iCurrentLayer++ )
	{
		m_pacScrollingLayers[iCurrentLayer].VReset();
	}

	for( int iCurrentLayer = 0; iCurrentLayer < m_kiNumAnimatedLayers; iCurrentLayer++ )
	{
		m_pacAnimatedLayers[iCurrentLayer].VReset();
	}
}

cocos2d::Scene& CParallax::GetScene() const
{
	return m_rcScene;
}