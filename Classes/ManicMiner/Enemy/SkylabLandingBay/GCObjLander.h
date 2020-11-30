////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October & Henrique Teixeira 2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJLANDER_H_
#define _GCOBJLANDER_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

//////////////////////////////////////////////////////////////////////////
//  This class defines an invidual instance of an enemy character.
//  Its purpose is to provide the functionality an enemy would require with regards to
//  velocity, direction flipping, horizontal/vertical axis alignment, bouncing on 
//  platform collision etc.
//
//////////////////////////////////////////////////////////////////////////

class CGCObjLander: public CGCObjSpritePhysics
{
private:

	enum class ELanderState 
	{
		Deploying,
		Exploding,
		Resetting
	};


	ELanderState				m_eLanderState;


	std::string					m_pszAnimation;
	std::string                 m_pszPlist;
	CGCFactoryCreationParams&	m_rFactoryCreationParams;

	const int					m_kaiDeployXSlots[4];
	const int					m_kiExplosionYSlot;
	int							m_iCurrentSlotIndex;
	float						m_fSpeed;

	float						m_fCurrentExplosionTime;


	//std::unique_ptr< CGCFactoryCreationParams > m_pCustomCreationParams;
	cocos2d::Animation* pAnimation;
public:
	CGCObjLander( const int ( &paiDeploySlots )[4], const int kiExplosionYSlot, const float kfTimeForFirstCollision, CGCFactoryCreationParams& ParamsInput);
	
	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjLander();
	
	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire	( void ) override;
	virtual void VOnResurrected		( void ) override;
	virtual void VOnUpdate			(float fTimeStep) override;
	virtual void VOnResourceRelease	() override;
	virtual void VOnReset() override;

	// TODO: ELanderState getter, will be used in the future so that these dont trigger contacts while moving back to next deploy position, invisible

	ELanderState GetState() const;
};
#endif // #ifndef _GCOBJLANDER_H_