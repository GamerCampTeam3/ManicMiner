#include "Parallax.h"

#include "ManicMiner/Player/CPlayer.h"
#include "2d/CCScene.h"
#include "ParallaxLayer.h"

CParallax::CParallax( const int kiNumLayers, cocos2d::Scene& rcScene, CPlayer& rcPlayer )
	: m_rcScene		( rcScene		)
	, m_rcPlayer	( rcPlayer		)
	, m_kiNumLayers	( kiNumLayers	)
	, m_pacLayers	( nullptr		)
	, m_iCurrentLayer ( 0 )
{
	m_pacLayers = new CParallaxLayer[ m_kiNumLayers ];
}

CParallax::~CParallax()
{
	if( m_pacLayers != nullptr )
	{
		delete[] m_pacLayers;
		m_pacLayers = nullptr;
	}
}

void CParallax::AddLayer( const SParallaxLayerData& rsLayerData )
{
	if( m_iCurrentLayer < m_kiNumLayers )
	{
		CParallaxLayer& rcCurrentLayer = m_pacLayers[ m_iCurrentLayer ];

		rcCurrentLayer.Init( m_rcScene, rsLayerData );
		m_iCurrentLayer++;
	}
	else
	{
		CCLOG( "EXCEEDED PARALLAX SPECIFIED NUMBER OF LAYERS: %d", m_kiNumLayers );
	}
}

void CParallax::Update()
{
	for( int iCurrentLayer = 0; iCurrentLayer < m_kiNumLayers; iCurrentLayer++ )
	{
		//m_pacLayers[ iCurrentLayer ].Update();
	}
}

void CParallax::Reset()
{
	for( int iCurrentLayer = 0; iCurrentLayer < m_kiNumLayers; iCurrentLayer++ )
	{
		//m_pacLayers[ iCurrentLayer ].Reset();
	}
}

cocos2d::Scene& CParallax::GetScene() const
{
	return m_rcScene;
}