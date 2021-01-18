//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Dave O'Dwyer October & Henrique Teixeira 2020											//
// This class defines the behaviour of the Enemy Lander, as seen in the original Manic Miner Level 14		//
//																					"Skylab Landing Bay"	//
//	https://youtu.be/BgUzteADsRI?t=890																		//
// Initially Dave's task, then moved over to Henrique for completion										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJLANDER_H_
#define _GCOBJLANDER_H_

#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

enum class ELanderState
{
	Deploying,
	Exploding,
	Resetting
};

class CGCObjLander: public CGCObjSpritePhysics
{
private:
	// Is the Lander Deploying, Exploding, or Resetting
	ELanderState				m_eLanderState;

	std::string					m_pszAnimation;
	std::string                 m_pszPlist;
	CGCFactoryCreationParams&	m_rFactoryCreationParams;

	// X coordinates where this lander will appear
	const int					m_kaiDeployXSlots[4];
	// Y coordinate where this lander will always start Exploding
	const int					m_kiExplosionYSlot;
	// Current X position in the m_kaiDeployXSlots array
	int							m_iCurrentSlotIndex;
	// Speed at which the lander travels downwards
	float						m_fSpeed;
	// Time elapsed since start of explosion
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

	ELanderState GetState() const;
};
#endif // #ifndef _GCOBJLANDER_H_