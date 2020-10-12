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

class CGCObjEnemy
: public CGCObjSpritePhysics
{

private:

	CGCCollisionManager		m_cGCCollisionManager;

public:
	CGCCollisionManager& GetCollisionManager()
	{
		return m_cGCCollisionManager;
	}

	void FlipEnemyDirection();
	void ResetFlipLatch();

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
	bool						bMovingAWayFromAnchorPoint;
	cocos2d::Vec2				TotalVelocity;
	const bool					k_bArtDefaultIsEnemyFacingRight;
	CGCFactoryCreationParams&	rFactoryCreationParams;
	bool						bInitialyMovingAwayFromAnchorPoint;
	bool                        bFlipisCurrentLatchedDisabled;

	bool CGCObjEnemy::checkForDirectionFlip();
	void CGCObjEnemy::SetFacingOrientation();
		
public:

	CGCObjEnemy(EMovementAxis EMovementAxisInput, cocos2d::Vec2 AnchorPoint, float MovementRange, float InitialDistanceFromAnchor, bool MovingAwayFromAnchorPoint, float Speed, 
				EEnemyIdentifier EnemyIdentifierInput, CGCFactoryCreationParams& ParamsInput);
	
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

	inline void SetAnimationAction(cocos2d::Animation* pAnimation)
	{
		RunAction(GCCocosHelpers::CreateAnimationActionLoop(pAnimation));
	}


	inline EEnemyIdentifier GetEnemyIdentifier()
	{
		return eEnemyIdentifier;
	}

};
#endif // #ifndef _GCOBJENEMY_H_