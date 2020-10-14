#ifndef _CMANICLAYER_H_
#define _CMANICLAYER_H_

#include <stdlib.h>

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "ManicMiner/Enums/ECollectibleTypeRequired.h"
#include "../GameState/EGameState.h"


////////////////////////////////////////////////////////////////////////////
// Forward Declarations													  //
////////////////////////////////////////////////////////////////////////////
class CGCObjSprite;														  //										
class CGCObjPlatform;													  //
class CGCObjGroupPlatform;												  //
class CGCObjItem;														  //
class CGCObjGroupItem;													  //
class CGCObjInvader;													  //
class CGCObjGroupInvader;												  //
class CGCObjGroupProjectilePlayer;										  //
class CGCObjEnemy;														  //
class CGCObjGroupEnemy;													  //
class CPlayer;															  //
class CCollectible;														  //
class CCollectiblesGroup;												  //
class CLevelManager;													  //
////////////////////////////////////////////////////////////////////////////

class CManicLayer : public IGCGameLayer, public b2ContactListener
{
private:
	// Reference to the GameInstance
	CLevelManager* m_pcLevelManager;
	
	// GameState Enum
	EGameState m_eGameState;

	// object groups
	CGCObjGroupPlatform* m_pcGCGroupPlatform;
	CGCObjGroupItem* m_pcGCGroupItem;
	CGCObjGroupProjectilePlayer* m_pcGCGroupProjectilePlayer;
	CGCObjGroupEnemy* m_pcGCGroupEnemy;

	// backgrounds
	CGCObjSprite* m_pcGCSprBackGround;

	// Mario
	CPlayer* m_pcPlayer;
	CCollectiblesGroup* m_pcCollectiblesGroup;

	ECollectibleTypeRequired m_eCollectibleTypeRequired;
	int m_iNumCollectiblesNeeded;
	int m_iNumSwitchesNeeded;


public:
	CManicLayer( void );
	virtual ~CManicLayer( void );

	//////////////////////////////////////////////////////////////////////////
	// player actions 
	enum EPlayerActions
	{
		EPA_Up = 0,
		EPA_Down,
		EPA_Left,
		EPA_Right,
		EPA_Jump
	};
	
	// player actions 
	//////////////////////////////////////////////////////////////////////////

	// called from VOnUpdate
	void HandleCollisions( void );

	//////////////////////////////////////////////////////////////////////////
	// CCNode interface...
	virtual void onEnter();
	// CCNode interface...
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer interface

	virtual	void VOnCreate( void )			override;
	virtual void VOnUpdate( f32 fTimeStep )	override;
	virtual	void VOnDestroy( void )			override;


	// IGCGameLayer interface
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// b2ContactListener interface - see b2ContactListener for details of 
	// when these get called and what they are

	virtual void BeginContact( b2Contact* pB2Contact );
	virtual void EndContact( b2Contact* pB2Contact );
	virtual void PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold );
	virtual void PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse );



	////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////

	CPlayer& GetPlayer() const;
	CLevelManager& GetLevelManager() const;


	////////////////////////////////////////////////////////////////////////////
	// Button
	////////////////////////////////////////////////////////////////////////////
	void CB_OnGameExitButton(Ref* pSender);


	private:


		////////////////////////////////////////////////////////////////////////////
		// Collision Events
		////////////////////////////////////////////////////////////////////////////
		void EnemyCollidedItem( CGCObjEnemy& rcEnemy, const b2Contact& rcContact );
		void PlayerCollidedInvader( CPlayer& rcPlayer, CGCObjInvader& rcInvader, const b2Contact& rcContact );
		void PlayerCollidedEnemy( CPlayer& rcPlayer, CGCObjEnemy& rcEnemy, const b2Contact& rcContact );
		void ItemCollected( CGCObjItem& rcItem, const b2Contact& rcContact );



		////////////////////////////////////////////////////////////////////////////
		// General Game Logic
		////////////////////////////////////////////////////////////////////////////
		void OnDeath();
		void OnFinishedLooting();
		void OnEscaped();
		void OutOfLives();













		////////////////////////////////////////////////////////////////////////// 
		// reset handling
		bool m_bResetWasRequested;

		void RequestReset()
		{
			m_bResetWasRequested = true;
		}

		void ResetRequestWasHandled()
		{
			m_bResetWasRequested = false;
		}

		bool ResetWasRequested()
		{
			return m_bResetWasRequested;
		}

		void ResetLevel();
};
#endif // #ifndef _CMANICLAYER_H_