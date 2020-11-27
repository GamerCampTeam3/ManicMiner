#include "ScrollingParallaxLayer.h"

#include "2d/CCActionInterval.h"

#include "ManicMiner/Player/CPlayer.h"
#include "ParallaxLayer.h"
#include "ScrollingParallaxLayer.h"

CScrollingParallaxLayer::CScrollingParallaxLayer()
	: CParallaxLayer()
	, m_pcPlayer	( nullptr )
	, m_fMovementScale ( 0.0f )
{

}

CScrollingParallaxLayer::~CScrollingParallaxLayer()
{

}

void CScrollingParallaxLayer::Init( cocos2d::Scene& pcScene, const SParallaxLayerData& rsData, const float kfMovementScale, CPlayer* pcPlayer )
{
	CParallaxLayer::Init( pcScene, rsData );
	m_pcPlayer = pcPlayer;
	m_fMovementScale = kfMovementScale * 0.005;
}

void CScrollingParallaxLayer::VUpdate()
{
	const float kfPlayerPosX = m_pcPlayer->GetSpritePosition().x;
	const float kfScreenCentreX = 960.0f;
	const float kfScreenCentreY = 540.0f;
	cocos2d::Vec2 v2PlayerOffsetFromCentre ( ( kfScreenCentreX - (  kfPlayerPosX - kfScreenCentreX ) * m_fMovementScale ), kfScreenCentreY );

	auto updateParallaxAction = cocos2d::MoveTo::create( 0.0f, v2PlayerOffsetFromCentre );
	GetSprite()->runAction( updateParallaxAction );
}

void CScrollingParallaxLayer::VReset()
{
	VUpdate();
}
