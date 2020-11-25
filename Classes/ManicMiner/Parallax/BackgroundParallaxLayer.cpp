#include "BackgroundParallaxLayer.h"

#include "2d/CCActionInterval.h"

#include "ManicMiner/Player/CPlayer.h"
#include "ParallaxLayer.h"
#include "ParallaxLayerData.h"

CBackgroundParallaxLayer::CBackgroundParallaxLayer()
	: CParallaxLayer()
	, m_pcPlayer	( nullptr )
{

}

CBackgroundParallaxLayer::~CBackgroundParallaxLayer()
{

}

void CBackgroundParallaxLayer::Init(cocos2d::Scene& pcScene, const SParallaxLayerData& rsData, CPlayer* pcPlayer)
{
	CParallaxLayer::Init(pcScene, rsData);
	m_pcPlayer = pcPlayer;
}

void CBackgroundParallaxLayer::VUpdate()
{
	const float kfPlayerPosX = m_pcPlayer->GetSpritePosition().x;
	const float kfScreenCentreX = 960.0f;
	const float kfScreenCentreY = 540.0f;
	cocos2d::Vec2 v2PlayerOffsetFromCentre ( ( kfScreenCentreX - (  kfPlayerPosX - kfScreenCentreX ) * GetData()->kfMovementScale ), kfScreenCentreY );

	auto updateParallaxAction = cocos2d::MoveTo::create( 0.0f, v2PlayerOffsetFromCentre );
	GetSprite()->runAction( updateParallaxAction );
}