////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJENEMY_H_
#define _GCOBJENEMY_H_

#ifndef _GCOBJSPRITEPHYSICS_H_
	#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif

#include <string>
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

namespace cocos2d
{
	class Sprite;
	class Animation;
	class ActionInterval;
}

//////////////////////////////////////////////////////////////////////////
// This is a sample class derived from CGCObject.
// 
// It could be the basis of your invader object, it's up to you really.
//
//////////////////////////////////////////////////////////////////////////
class CGCObjEnemy
: public CGCObjSpritePhysics
{

public: 

	enum EMovementAxis { EMovementAxis_UpDown, EMovementAxis_LeftRight };
	enum EEnemyIdentifier { EEnemyIdentifier_Type1, EEnemyIdentifier_Type2 };

private:

	EMovementAxis				eMovementAxis;
	EEnemyIdentifier			eEnemyIdentifier;
	float						fSpeed;
	float						fInitialDistanceFromAnchor;
	cocos2d::Vec2				cAnchorPoint;
	float						fMovementWindowLength;
	bool						bMovingAWayFromStartPosition;
	cocos2d::Vec2				TotalVelocity;
	const bool					k_bArtDefaultIsEnemyFacingRight;
	CGCFactoryCreationParams&	rFactoryCreationParams;

	bool CGCObjEnemy::checkForDirectionFlip();
	void CGCObjEnemy::SetFacingOrientation();
	
	cocos2d::Animation* pAnimation;

public:

	CGCObjEnemy(EMovementAxis EMovementAxisInput, cocos2d::Vec2 AnchorPoint, float MovementWindowLength, float InitialDistanceFromAnchor, float SpeedInput, EEnemyIdentifier EnemyIdentifierInput, CGCFactoryCreationParams& ParamsInput,
				cocos2d::Animation* pAnimationInput);
	
	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjEnemy()
	{}

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;

	virtual void VOnResurrected		( void ) override;

	virtual void CGCObjEnemy::VOnUpdate(float fTimeStep) override;


	inline void SetAnimationAction()
	{
		//  NOT USED AT PRESENT.

		//pItemSprite->RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation1));

		RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));
	}


	inline EEnemyIdentifier GetEnemyIdentifier()
	{
		return eEnemyIdentifier;
	}

};
#endif // #ifndef _GCOBJENEMY_H_