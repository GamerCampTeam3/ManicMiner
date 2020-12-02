#include "AnimatedParallaxLayer.h"
#include "2d/CCActionInterval.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

CAnimatedParallaxLayer::CAnimatedParallaxLayer()
	: m_v2InitialPosition(cocos2d::Vec2(960.f, 540.f))
	, m_bHasNotFoundNewPoint(true)
	, m_ftravelSpeed(10.f)
	, m_bShouldStopMoving(false)
	, m_bMovingRight(true)
{
	m_v2Right = cocos2d::Vec2(400.f, 0.f);
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

	/*
	for (float i = 0; i < 1; i += 0.01f)
	{
		int x1 = 0;
		int x2 = 200;
		int x3 = 400;

		int y1 = 0;
		int y2 = 200;
		int y3 = 0;

		// The Green Line
		int xa = getPt(x1, x2, i);
		int ya = getPt(y1, y2, i);
		int xb = getPt(x2, x3, i);
		int yb = getPt(y2, y3, i);

		// The Black Dot
		m_v2Right.x = getPt(xa, xb, i);
		m_v2Right.y = getPt(ya, yb, i);

		auto updateParallaxAction2 = cocos2d::MoveTo::create(0.0f, m_v2Right);
		GetSprite()->runAction(updateParallaxAction2);

	}*/
	
	//auto updateParallaxAction2 = cocos2d::MoveTo::create(10.0f, m_v2Right);
	//GetSprite()->runAction(updateParallaxAction2);
	
	//cocos2d::RotateBy::create()
}

void CAnimatedParallaxLayer::VUpdate()
{

	/*
	if(!m_bShouldStopMoving)
	{

		for (float i = 0; i < 1; i += 0.01f)
		{
			int x1 = m_v2Right.x;
			int x2 = 1160;
			int x3 = 1260;

			int y1 = m_v2Right.y;
			int y2 = 640;
			int y3 = 540;
			
			// The Green Line
			int xa = getPt(x1, x2, i);
			int ya = getPt(y1, y2, i);
			int xb = getPt(x2, x3, i);
			int yb = getPt(y2, y3, i);

			// The Black Dot
			m_v2Right.x = getPt(xa, xb, i);
			m_v2Right.y = getPt(ya, yb, i);

			auto updateParallaxAction = cocos2d::MoveTo::create(0.0f, m_v2Right);
			GetSprite()->runAction(updateParallaxAction);
		}

		m_bShouldStopMoving = true;
	}*/
	
	//CCLOG("Fish Position X: %d", GetSprite()->getPosition().x);
	//CCLOG("Fish Position Y: %d", GetSprite()->getPosition().y);
	//m_v2InitialPosition.x++;
	
	if(m_bMovingRight)
	{
		m_v2InitialPosition.x += 1;
		m_v2InitialPosition.y += cocos2d::random(-1.f, 1.f);
		if (m_v2InitialPosition.x >= 1360.f)
		{
			m_bMovingRight = false;
			GetSprite()->setFlippedX(true);
		}
	}
	else
	{
		m_v2InitialPosition.x -= 1;
		m_v2InitialPosition.y += cocos2d::random(-1.f, 1.f);
		if (m_v2InitialPosition.x <= 560.f)
		{
			m_bMovingRight = true;
			GetSprite()->setFlippedX(false);
		}
	}
	auto updateParallaxAction = cocos2d::MoveTo::create(0.0f, m_v2InitialPosition);
	GetSprite()->runAction(updateParallaxAction);


	
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
			auto updateParallaxAction2 = cocos2d::MoveTo::create(2.0f, m_v2NextMoveToPosition);
			GetSprite()->runAction(updateParallaxAction2);
			
			// has found new point
			m_bHasNotFoundNewPoint = false;
		}
		
	}*/
}

void CAnimatedParallaxLayer::VReset()
{
	// Make new random animation
	const float kfScreenCentreX = 960.0f;
	const float kfScreenCentreY = 540.0f;
	cocos2d::Vec2 v2CentreScreen = cocos2d::Vec2(kfScreenCentreX, kfScreenCentreY);

	auto updateParallaxAction = cocos2d::MoveTo::create(0.0f, v2CentreScreen);
	GetSprite()->runAction(updateParallaxAction);
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

int CAnimatedParallaxLayer::getPt(int n1, int n2, float perc)
{
	int diff = n2 - n1;
	return n1 + (diff * perc);
}
