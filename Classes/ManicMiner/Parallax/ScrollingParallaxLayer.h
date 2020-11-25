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
	CPlayer* m_pcPlayer;
	float m_fMovementScale;
};
#endif // #ifndef _SCROLLING_PARALLAX_LAYER_H_