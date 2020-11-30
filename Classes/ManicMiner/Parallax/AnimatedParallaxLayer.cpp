#include "AnimatedParallaxLayer.h"
#include "2d/CCActionInterval.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

CAnimatedParallaxLayer::CAnimatedParallaxLayer()
{
	
}

CAnimatedParallaxLayer::~CAnimatedParallaxLayer()
{

}

void CAnimatedParallaxLayer::Init(cocos2d::Scene& pcScene, const SParallaxLayerData& rsData)
{
	CParallaxLayer::Init(pcScene, rsData);
	GetSprite()->setContentSize(cocos2d::Size(100.f, 100.f));
	/*
	m_pcVisualElementSprite->CreateSprite(rsData.kpszPlist_image);
	m_pcVisualElementSprite->SetResetPosition(cocos2d::Vec2(100.f, 200.f));
		m_pcVisualElementSprite->SetSpriteScale(10.f, 10.f);*/
}

void CAnimatedParallaxLayer::VUpdate()
{
}

void CAnimatedParallaxLayer::VReset()
{
	// Make new random animation
}

void CAnimatedParallaxLayer::FindRandomMoveToPosition()
{
	cocos2d::Vec2 RandomPosition;

	const int kRandXMin = 200;
	const int kRandXMax = 1720;

	const int kRandYMin = 300;
	const int kRandYMax = 680;
	
	const int kRandX = cocos2d::random(kRandXMin, kRandXMax);
	const int kRandY = cocos2d::random(kRandYMin, kRandYMax);

	RandomPosition = cocos2d::Vec2(kRandX, kRandY);

	m_v2NextMoveToLocation = RandomPosition;
}

void CAnimatedParallaxLayer::GetDistanceToCurrentMoveToPosition(bool bInitialDistance)
{
	const cocos2d::Vec2 currentPosition = GetSprite()->getPosition();
	
	if (bInitialDistance)
	{
		m_v2CurrentMoveToLocation = m_v2NextMoveToLocation;

		m_fInitialDistanceToMoveToPosition = currentPosition.getDistance(m_v2CurrentMoveToLocation);
	}
	else
	{
		m_f
	}
}
