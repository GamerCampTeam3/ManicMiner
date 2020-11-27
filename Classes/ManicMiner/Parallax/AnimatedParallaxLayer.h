#ifndef _ANIMATED_PARALLAX_LAYER_H
#define _ANIMATED_PARALLAX_LAYER_H

#include "ParallaxLayer.h"

class CAnimatedParallaxLayer : public CParallaxLayer
{
public:
	CAnimatedParallaxLayer();
	virtual ~CAnimatedParallaxLayer();


	virtual void VUpdate() override;
	virtual void VReset() override;


	//auto updateParallaxAction = cocos2d::MoveTo::create( 0.0f, v2PlayerOffsetFromCentre );
	//GetSprite()->runAction( updateParallaxAction );

	//auto goUp = MoveBy::create( 4, Vec2( 0, -250 ) );
	//auto goDown = goUp->reverse();
	//auto go = MoveBy::create( 8, Vec2( -500, 0 ) );
	//auto goBack = go->reverse();
	//auto seq = Sequence::create( goUp, go, goDown, goBack, nullptr );
	//parallax->runAction( ( RepeatForever::create( seq ) ) )
private:

};

#endif // #ifndef _ANIMATED_PARALLAX_LAYER_H