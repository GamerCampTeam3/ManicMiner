#ifndef _CTESTLAYER_
#define _CTESTLAYER_

#include "GamerCamp/GameSpecific/GCGameLayerPlatformer.h"


#include "../GameState/EGameState.h"

//class CGCObjItem;

class CTestLayer : public CGCGameLayerPlatformer
{
private:
    EGameState m_eGameState;
	
	// Should go on player class
	int m_iLivesLeft;

	// Should be reference to the item obj group actually, which manages the num of collected items
	int m_iCollected;

public:
    CTestLayer();
    virtual  ~CTestLayer();

	void VOnCreate() override;
    void VOnUpdate( f32 fTimeStep ) override;

    void OnFinishedLooting();
    void OnEscaped();
	void OnPlayerDeath( CGCObjPlayer& rPlayer );
	void OutOfLives();



	void PlayerCollidedInvader( CGCObjPlayer& rPlayer, CGCObjInvader& rInvader, const b2Contact& rcContact );
	void ItemCollected( CGCObjItem& rItem, const b2Contact& rcContact );


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	EGameState GetGameState();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setters
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetGameState( EGameState eGameState );


};


#endif //#ifndef _CTESTLAYER_