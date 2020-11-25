#include "Parallax.h"

#include "2d/CCScene.h"

#include "BackgroundParallaxLayer.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ParallaxLayer.h"
#include "base/ccMacros.h"

CParallax::CParallax( const int kiNumLayers, cocos2d::Scene& rcScene, CPlayer& rcPlayer )
	: m_rcScene		( rcScene		)
	, m_rcPlayer	( rcPlayer		)
	, m_kiNumLayers	( kiNumLayers	)
	, m_pacBGPLayers( nullptr		)
	, m_iCurrentLayer ( 0 )
{
	m_pacBGPLayers = new CBackgroundParallaxLayer[ m_kiNumLayers ];
}

CParallax::~CParallax()
{
	if(m_pacBGPLayers != nullptr )
	{
		delete[] m_pacBGPLayers;
		m_pacBGPLayers = nullptr;
	}
}

void CParallax::AddLayer( const SParallaxLayerData& rsLayerData )
{
	CCAssert( (m_iCurrentLayer < m_kiNumLayers), "Trying to add more layers than allocated to CParallax" );
	CCAssert( rsLayerData.kpszPlist_image != nullptr, "CParallaxLayer pszPlist incorrect path" );

	CBackgroundParallaxLayer& rcCurrentLayer = m_pacBGPLayers[m_iCurrentLayer];

	rcCurrentLayer.Init( m_rcScene, rsLayerData, &m_rcPlayer );
	m_iCurrentLayer++;
}

void CParallax::Update()
{
	for( int iCurrentLayer = 0; iCurrentLayer < m_kiNumLayers; iCurrentLayer++ )
	{
		m_pacBGPLayers[ iCurrentLayer ].Update();
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