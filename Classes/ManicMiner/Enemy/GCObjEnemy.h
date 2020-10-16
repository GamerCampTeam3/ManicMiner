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
#include "ManicMiner/Enums/EEnemyTypes.h"

namespace cocos2d
{
	class Sprite;
	class Animation;
	class ActionInterval;
}

class CGCObjEnemy
: public CGCObjSpritePhysics
{

public:

	void BounceEnemyDirection();
	
	enum EMovementAxis { EMovementAxis_UpDown, EMovementAxis_LeftRight };

private:

	EMovementAxis				eMovementAxis;
	EnemyTypes::EEnemyId	    eEnemyIdentifier;
	cocos2d::Vec2				cAnchorPoint;
	cocos2d::Vec2				TotalVelocity;
	float						fSpeed;
	float						fInitialDistanceFromAnchor;
	float						fMovementWindowLength;
	bool						bMovingAWayFromAnchorPoint;
	bool						bInitialyMovingAwayFromAnchorPoint;
	bool                        bBounceIsLatchedDisabled;
	bool						bSpriteIsFlippable;
	bool						m_bHasBeenCollided;
	const bool					k_bArtDefaultIsEnemyFacingRight;
	CGCFactoryCreationParams&	rFactoryCreationParams;

	bool CGCObjEnemy::CheckForDirectionFlip();
	void CGCObjEnemy::SetFacingOrientation();
		
public:

	CGCObjEnemy(EMovementAxis EMovementAxisInput, cocos2d::Vec2 AnchorPoint, float MovementRange, float InitialDistanceFromAnchor, bool MovingAwayFromAnchorPoint, float Speed, bool SpriteIsFlippable, 
		EnemyTypes::EEnemyId EnemyIdentifierInput, CGCFactoryCreationParams& ParamsInput);
	
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

	// Bib Edit
	const bool GetHasCollided() { return m_bHasBeenCollided; };
	void SetHasCollided( bool collided ) { m_bHasBeenCollided = collided; };
	
	inline  EnemyTypes::EEnemyId GetEnemyIdentifier()
	{
		return eEnemyIdentifier;
	}
};
#endif // #ifndef _GCOBJENEMY_H_