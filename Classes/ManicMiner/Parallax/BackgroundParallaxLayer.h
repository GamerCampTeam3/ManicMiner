#ifndef _BACKGROUND_PARALLAX_LAYER_H_
#define _BACKGROUND_PARALLAX_LAYER_H_

#include "ParallaxLayer.h"

class CPlayer;

class CBackgroundParallaxLayer : public CParallaxLayer
{
public:
	CBackgroundParallaxLayer();
	virtual ~CBackgroundParallaxLayer();


	virtual void Init(cocos2d::Scene& pcScene, const SParallaxLayerData& rsData, CPlayer* pcPlayer);
	virtual void Update() override;
private:
	CPlayer* m_pcPlayer;
};
#endif // #ifndef _BACKGROUND_PARALLAX_LAYER_H_