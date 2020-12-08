#include "CMovingDoor.h"

#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Doors/CBlock.h"
#include "GamerCamp/GCCocosInterface/SGCFactoryCreationParams.h"
#include "ManicMiner/GameManager/CGameManager.h"



CMovingDoor::CMovingDoor( CGameManager& rcGameManager, const CGCFactoryCreationParams& rcFactoryCreationParams )
	: m_pcGameManager	( &rcGameManager )
	, m_pcUpperBlock	( nullptr )
	, m_pcLowerBlock	( nullptr )
	, m_cCreationParams	( rcFactoryCreationParams )
{
	Init();
}

// Initialize the 2 blocks here.
void CMovingDoor::Init()
{
	// Since we are not using OGMO for this, we must manually calculate the position
	// PTM (Pixels per  meter) * The block where we want it + OFFSET (single block)
	// Since it's a "Door", they are both aligned on the same X pos, with one block different on Y
	const float xPos	= m_kfPTM * 17.0f + m_kfOffset;							   
	const float yPos0	= m_kfPTM * 3.0f  + m_kfOffset;
	const float yPos1	= m_kfPTM * 4.0f  + m_kfOffset;

	// Using the previous values, we create vectors.
	m_pcUpperBlock = new CBlock(  m_cCreationParams, cocos2d::Vec2( xPos, yPos0 ) );
	m_pcLowerBlock = new CBlock(  m_cCreationParams, cocos2d::Vec2( xPos, yPos1 ) );

	// And we set this on the GameManager
	if (m_pcGameManager != nullptr )
	{
		m_pcGameManager->SetMovingDoors( this );
	}
}

CMovingDoor::~CMovingDoor()
{

}

// This will be called, when the first switch is flipped.
void CMovingDoor::MoveBlocksAside() const
{
	m_pcUpperBlock->DissolveBlock();
	m_pcLowerBlock->DissolveBlock();
}


//----------------------------- GROUP OBJECT HANDLING ----------------------------------//
GCTypeID CMovingDoor::VGetTypeId()														//
{																						//
	return GetGCTypeIDOf( CBlock );														//
}																						//
																						//
bool CMovingDoor::VHandlesThisTypeId( GCTypeID idQueryType )							//
{																						//
	return(GetGCTypeIDOf( CBlock ) == idQueryType);										//
}																						//
																						//
void CMovingDoor::VOnGroupResourceAcquire()												//
{																						//
	CGCObjectGroup::VOnGroupResourceAcquire();											//
	//Init();																				//
}																						//
																						//
void CMovingDoor::VOnGroupResourceAcquire_PostObject()									//
{																						//
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();								//
}																						//
																						//
// On Delete, destroy objects															//
void CMovingDoor::VOnGroupResourceRelease()												//
{																						//
	CGCObjectGroup::VOnGroupResourceRelease();											//
	DestroyItems();																		//
}																						//
																						//
void CMovingDoor::DestroyItems()														//
{																						//
	DestroyObjectsReverseOrder( [&]( CGCObject* pObject )	//
	{																					//
		GCASSERT( GetGCTypeIDOf( CBlock ) == pObject->GetGCTypeID(), "wrong type!" );	//
	} );																				//
}																						//
																						//
//--------------------------------------------------------------------------------------//