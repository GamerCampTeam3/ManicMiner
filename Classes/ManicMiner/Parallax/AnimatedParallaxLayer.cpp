#include "AnimatedParallaxLayer.h"
#include "2d/CCActionInterval.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

CAnimatedParallaxLayer::CAnimatedParallaxLayer()
	: m_v2InitialPosition(cocos2d::Vec2(960.f, 540.f))
	, m_bHasNotFoundNewPoint(true)
	, m_ftravelSpeed(10.f)
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

	auto updateParallaxAction2 = cocos2d::MoveTo::create(10.0f, m_v2NextMoveToPosition);
	GetSprite()->runAction(updateParallaxAction2);
	
	//cocos2d::RotateBy::create()
}

void CAnimatedParallaxLayer::VUpdate()
{
	//m_v2InitialPosition.x++;
	//auto updateParallaxAction = cocos2d::MoveTo::create(0.0f, m_v2InitialPosition);
	//GetSprite()->runAction(updateParallaxAction);


	
	/*
	float Distance = GetDistanceToCurrentMoveToPosition(false);
	
	if(Distance <= m_fInterpolatingDistance)
	{
		// find new point
		if(m_bHasNotFoundNewPoint)
		{
			// don't forget to reset bool when finish interpolating

			GetSprite()->stopAllActions();
			FindRandomMoveToPosition();
			GetDistanceToCurrentMoveToPosition(true);
			auto updateParallaxAction2 = cocos2d::MoveTo::create(10.0f, m_v2NextMoveToPosition);
			GetSprite()->runAction(updateParallaxAction2);
			
			// has found new point
			m_bHasNotFoundNewPoint = false;
		}
		
	}*/
}

void CAnimatedParallaxLayer::VReset()
{
	// Make new random animation
}

void CAnimatedParallaxLayer::FindRandomMoveToPosition()
{
	cocos2d::Vec2 RandomPosition;
	cocos2d::Vec2 CurrentSpritePosition;

	// check whether randomposition - currentposition will place the fish outside the bounds for every side, then give specified new position
	
	// X and Y Values clamped to a set range so that the fish doesn't swim under the ground or behind the walls on the left/right side of the screen
	// the min and max amount of distance the fish can move on every moveto action
	const int kRandXMin = -300;
	const int kRandXMax = 300;

	const int kRandYMin = -300;
	const int kRandYMax = 300;
	
	const int kRandX = cocos2d::random(kRandXMin, kRandXMax);
	const int kRandY = cocos2d::random(kRandYMin, kRandYMax);

	RandomPosition = cocos2d::Vec2(kRandX, kRandY);
	CurrentSpritePosition = GetSprite()->getPosition();

	m_v2NextMoveToPosition = RandomPosition - CurrentSpritePosition;
}

float CAnimatedParallaxLayer::GetDistanceToCurrentMoveToPosition(bool bInitialDistance)
{
	float fDistanceToReturn = 0.f;
	const cocos2d::Vec2 currentPosition = GetSprite()->getPosition();
	
	if (bInitialDistance)
	{
		m_v2CurrentMoveToPosition = m_v2NextMoveToPosition;

		fDistanceToReturn = currentPosition.getDistance(m_v2CurrentMoveToPosition);
		
		//fDistanceToReturn = sqrt(() + ());
			//sqrt()
	}
	else
	{
		fDistanceToReturn = currentPosition.getDistance(m_v2CurrentMoveToPosition);
	}

	// get absolute value
	if(fDistanceToReturn < 0)
	{
		fDistanceToReturn *= -1;
	}
	return fDistanceToReturn;
}

void CAnimatedParallaxLayer::FindDistanceToStartInterpolatingTowardsNewPoint()
{
	const float currentDistanceToPoint = GetDistanceToCurrentMoveToPosition(false);

	// start interpolating at 2/3 of distance so (distance*2)/3 to get the fraction
	m_fInterpolatingDistance = (currentDistanceToPoint*2)/3;
}
