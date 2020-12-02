#ifndef _ANIMATED_PARALLAX_LAYER_H
#define _ANIMATED_PARALLAX_LAYER_H

#include "ParallaxLayer.h"

enum class EAIState
{
	Idle,
	Moving,
	Interpolating,
};

class CAnimatedParallaxLayer : public CParallaxLayer
{
public:
	CAnimatedParallaxLayer();
	virtual ~CAnimatedParallaxLayer();


	void Init(cocos2d::Scene& pcScene, const SParallaxLayerData& rsData);
	
	void VUpdate() override;
	virtual void VReset() override;


	//auto updateParallaxAction = cocos2d::MoveTo::create( 0.0f, v2PlayerOffsetFromCentre );
	//GetSprite()->runAction( updateParallaxAction );

	//auto goUp = MoveBy::create( 4, Vec2( 0, -250 ) );
	//auto goDown = goUp->reverse();
	//auto go = MoveBy::create( 8, Vec2( -500, 0 ) );
	//auto goBack = go->reverse();
	//auto seq = Sequence::create( goUp, go, goDown, goBack, nullptr );
	//parallax->runAction( ( RepeatForever::create( seq ) ) )

	// Returns random 2DVector Position in range of X = (min = 200, max = 1720) and Y = (min = 300, max = 680)
	void FindRandomMoveToPosition();

	float GetDistanceToCurrentMoveToPosition(bool bInitialDistance);

	void FindDistanceToStartInterpolatingTowardsNewPoint();

	float			m_fInitialDistanceToMoveToPosition;
	float			m_fCurrentDistanceToMoveToPosition;
	float			m_fInterpolatingDistance;
	float			m_ftravelSpeed;
	cocos2d::Vec2	m_v2CurrentMoveToPosition;
	cocos2d::Vec2	m_v2NextMoveToPosition;
	cocos2d::Vec2	m_v2InitialPosition;

	bool m_bHasNotFoundNewPoint;

	bool m_bShouldStopMoving;
	bool m_bMovingRight;
	
	// TEST
	cocos2d::Vec2 m_v2Right;

	int getPt(int n1, int n2, float perc);
	
private:

};

#endif // #ifndef _ANIMATED_PARALLAX_LAYER_H