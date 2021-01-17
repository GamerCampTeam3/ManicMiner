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

private:

	// whether the Animated Parallax Layer (Fish)
	// should move in the right or left direction
	// true = right || false = left
	bool m_bMovingRight;

	// The Position that this Animated Parallax Layer
	// is supposed to move to on this frame in
	// the 2D Game World (2D Coordinate System)
	cocos2d::Vec2	m_v2InitialPosition;
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// EXPERIMENTAL PATH FINDING - NOT BEING USED

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

	bool m_bHasNotFoundNewPoint;

	bool m_bShouldStopMoving;

	int getPt(int n1, int n2, float perc);
	
	// EXPERIMENTAL PATH FINDING - NOT BEING USED
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
private:

};

#endif // #ifndef _ANIMATED_PARALLAX_LAYER_H