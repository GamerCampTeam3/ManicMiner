// -------------------------------------------------------------------------------------------------------------------- //
// Gamer Camp 2020 / Henrique Teixeira																					//
// -------------------------------------------------------------------------------------------------------------------- //
// Purpose	:	This class manages the creation of a ScrollingParallaxLayer, which is a type of layer that makes up		//
//				the visible background, and moves around horizontally, in opposed direction to the player movement		//
// -------------------------------------------------------------------------------------------------------------------- //

#ifndef _SCROLLING_PARALLAX_LAYER_H_
#define _SCROLLING_PARALLAX_LAYER_H_

#include "ParallaxLayer.h"

class CPlayer;

class CScrollingParallaxLayer : public CParallaxLayer
{
public:
	CScrollingParallaxLayer();
	virtual ~CScrollingParallaxLayer();

	void Init( cocos2d::Scene& pcScene, const SParallaxLayerData& rsData, float fMovementScale, CPlayer* pcPlayer );

	virtual void VUpdate() override;
	virtual void VReset() override;
private:
	// Pointer to player, needed for calculating screen position
	CPlayer* m_pcPlayer;

	// Movement Scale to calculate how far this layer should move
	float m_fMovementScale;
};
#endif // #ifndef _SCROLLING_PARALLAX_LAYER_H_