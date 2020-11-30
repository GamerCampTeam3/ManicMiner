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

	const float kfScreenCentreX = 960.0f;
	const float kfScreenCentreY = 540.0f;
	cocos2d::Vec2 v2CentreScreen = cocos2d::Vec2(kfScreenCentreX, kfScreenCentreY);
	auto updateParallaxAction = cocos2d::MoveTo::create(0.0f, v2CentreScreen);
	GetSprite()->runAction(updateParallaxAction);
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
		m_fInitialDistanceToMoveToPosition = currentPosition.getDistance(m_v2CurrentMoveToLocation);
	}
}
