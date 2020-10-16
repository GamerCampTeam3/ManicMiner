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
	cocos2d::Vec2				m_cAnchorPoint;
	cocos2d::Vec2				m_cTotalVelocity;
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



	CGCObjEnemy(const EMovementAxis EMovementAxisInput, const cocos2d::Vec2& AnchorPoint, const float fMovementRange, const float fInitialDistanceFromAnchor,
		bool bMovingAwayFromAnchorPoint, const float fSpeed, const bool bSpriteIsFlippable, const EnemyTypes::EEnemyId EnemyIdentifierInput,
		CGCFactoryCreationParams& ParamsInput);

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