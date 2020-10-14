#ifndef _CMANICLAYER_H_
#define _CMANICLAYER_H_

#include <stdlib.h>

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

////////////////////////////////////////////////////////////////////////////
// Forward Declarations													  //
////////////////////////////////////////////////////////////////////////////
class CGameInstance;													  //
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
class CGCObjLander;														  //
class CGCObjGroupLander;												  //
class CPlayer;
class CCollectible;
//
////////////////////////////////////////////////////////////////////////////

class CManicLayer : public IGCGameLayer, public b2ContactListener
{
private:
	// Reference to the GameInstance
	CGameInstance& m_rGameInstance;
	
	// object groups
	CGCObjGroupPlatform* m_pcGCGroupPlatform;
	CGCObjGroupItem* m_pcGCGroupItem;
	CGCObjGroupProjectilePlayer* m_pcGCGroupProjectilePlayer;
	CGCObjGroupEnemy* m_pcGCGroupEnemy;
	CGCObjGroupLander* m_pcGCGroupLander;

	// backgrounds
	CGCObjSprite* m_pcGCSprBackGround;

	// Mario
	CPlayer* m_pcPlayer;
	CCollectible* m_pCollectibleTest;

public:
	CManicLayer( void );
	CManicLayer( CGameInstance& rGameInstance );
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

	void EnemyCollidedItem(CGCObjEnemy& rEnemy, const b2Contact& rcContact);

	

	void PlayerCollidedInvader( CPlayer& rPlayer, CGCObjInvader& rInvader, const b2Contact& rcContact );
	void PlayerCollidedEnemy(CPlayer& rPlayer, CGCObjEnemy& rEnemy, const b2Contact& rcContact);

	void ItemCollected( CGCObjItem& rItem, const b2Contact& rcContact );


	////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////

	CPlayer& GetPlayer();

	void CB_OnGameExitButton(Ref* pSender);


};
#endif // #ifndef _CMANICLAYER_H_