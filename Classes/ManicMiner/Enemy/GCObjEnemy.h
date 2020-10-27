////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJENEMY_H_
#define _GCOBJENEMY_H_

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "ManicMiner/Enums/EEnemyTypes.h"

//////////////////////////////////////////////////////////////////////////
//  This class defines an invidual instance of an enemy character.
//  Its purpose is to provide the functionality an enemy would require with regards to
//  velocity, direction flipping, horizontal/vertical axis alignment, bouncing on 
//  platform collision etc.
//
//////////////////////////////////////////////////////////////////////////

class CGCObjEnemy
: public CGCObjSpritePhysics
{
private:
	EnemyTypes::EMovementAxis	m_eMovementAxis;
	EnemyTypes::EEnemyId	    m_eEnemyId;
	cocos2d::Vec2				m_cAnchorPoint;
	cocos2d::Vec2				m_cTotalVelocity;
	float						m_fSpeed;
	float						m_fInitialDistanceFromAnchor;    
	float						m_fMovementWindowLength;
	bool						m_bMovingAWayFromAnchorPoint;
	bool						m_bInitialyMovingAwayFromAnchorPoint;
	bool                        m_bBounceCollisionDisabled;
	bool						m_bSpriteIsFlippable;
	bool						m_bHasBeenCollided;
	CGCFactoryCreationParams&	m_rFactoryCreationParams;

	bool CheckForBoundaryReached(const float fCurrentPosition, const float fAnchorPoint, const float fMovementWindowLength);
	bool CheckForDirectionFlip	();
	void SetFacingOrientation	();
		
public:
	CGCObjEnemy(const EnemyTypes::EMovementAxis EMovementAxisInput, const cocos2d::Vec2& rcAnchorPoint, const float fMovementRange, const float fInitialDistanceFromAnchor,
		bool bMovingAwayFromAnchorPoint, const float fSpeed, const bool bSpriteIsFlippable, const EnemyTypes::EEnemyId eEnemyIdentifier,
		CGCFactoryCreationParams& ParamsInput);

	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjEnemy();
	
	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;
	virtual void VOnResurrected		( void ) override;
	virtual void VOnUpdate			(float fTimeStep) override;
	void BounceEnemyDirection();

	inline  EnemyTypes::EEnemyId GetEnemyIdentifier()
	{
		return m_eEnemyId;
	}
};
#endif // #ifndef _GCOBJENEMY_H_