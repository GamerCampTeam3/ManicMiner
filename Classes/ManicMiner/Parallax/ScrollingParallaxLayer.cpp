// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //

// Scalar used to multiply the incoming kfMovementScale when creating this ScrollingParallaxLayer
// Just makes the creation of the layers use nicer, rounder floats - [0.0f, 10.0f] ish
static const float s_kfMovementScaleMultiplier = 0.005f;

#include "ScrollingParallaxLayer.h"

#include "2d/CCActionInterval.h"
#include "ManicMiner/Player/CPlayer.h"
#include "ParallaxLayer.h"
#include "ScrollingParallaxLayer.h"

CScrollingParallaxLayer::CScrollingParallaxLayer()
	: CParallaxLayer()
	, m_pcPlayer	( nullptr )
	, m_fMovementScale ( 0.0f )
{}

CScrollingParallaxLayer::~CScrollingParallaxLayer()
{}

void CScrollingParallaxLayer::Init( cocos2d::Scene& pcScene, const SParallaxLayerData& rsData, const float kfMovementScale, CPlayer* pcPlayer )
{
	// Call parent Init
	CParallaxLayer::Init( pcScene, rsData );
	// Assign pointer to player, will be needed to adjust position
	m_pcPlayer = pcPlayer;
	// Calculate real move scale
	m_fMovementScale = kfMovementScale * s_kfMovementScaleMultiplier;
}

void CScrollingParallaxLayer::VUpdate()
{
	// Calculate how far the player is from the centre of the screen
	const float kfPlayerPosX = m_pcPlayer->GetSpritePosition().x;
	const float kfScreenCentreX = 960.0f;
	const float kfScreenCentreY = 540.0f;
	cocos2d::Vec2 v2PlayerOffsetFromCentre ( ( kfScreenCentreX - (  kfPlayerPosX - kfScreenCentreX ) * m_fMovementScale ), kfScreenCentreY );

	// Run action, make this layer move towards X = v2PlayerOffsetFromCentre
	auto updateParallaxAction = cocos2d::MoveTo::create( 0.0f, v2PlayerOffsetFromCentre );
	GetSprite()->runAction( updateParallaxAction );
}

void CScrollingParallaxLayer::VReset()
{
	VUpdate();
}
