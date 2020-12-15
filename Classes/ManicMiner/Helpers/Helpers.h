//////////////////
/// Bib
//////////////////
///
#ifndef HELPERS_H
#define HELPERS_H

#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "ManicMiner/Layers/CManicLayer.h"
#include "ManicMiner/Platforms/CTriggerPlatform.h"
#include "ManicMiner/Enemy/GCObjEnemy.h"
#include "ManicMiner/Doors/CDoor.h"

// A template to allow for ease of deletion of pointers and array of pointers
template <typename T> void safeDelete( T*& a )
{
	delete a;
	a = nullptr;
}

template <typename T> void safeDeleteArray( T*& a )
{
	delete[] a;
	a = nullptr;
}

namespace EasySprite
{
	/// <summary>
	/// This is a function that creates a sprite for convenience purposes. It returns a pointer to the sprite created.
	/// </summary>
	/// <param name="pszPlist">The plist that the sprite will use (texture).</param>
	/// <param name="fZOrder">The Order in the screen ZOrder at which it will be placed.</param>
	/// <param name="fXScale">The Width scale of the sprite.</param>
	/// <param name="fYScale">The Height scale of the sprite.</param>
	/// <param name="kv2SpritePos">The Position at which the sprite will be created at (cocos2d::Vec2) </param>
	/// <param name="rcCurrentLayer">The current parent layer of the sprite.</param>
	/// <param name="pszName">The Name for the sprite, used if you wish to remove it. </param>
	/// <returns> The pointer to the CGCObjSprite it created for you.</returns>
	inline CGCObjSprite* Create(  const char* pszPlist, float fZOrder, float fXScale, float fYScale, const cocos2d::Vec2 kv2SpritePos, CManicLayer* rcCurrentLayer, const char* pszName = "none")
	{
		CGCObjSprite* pcSprite = new CGCObjSprite();
		
		pcSprite->CreateSprite( pszPlist );
		pcSprite->SetSpriteGlobalZOrder( fZOrder );
		pcSprite->SetSpriteScale( fXScale, fYScale );
		pcSprite->SetResetPosition( kv2SpritePos );
		pcSprite->GetSprite()->setPosition( pcSprite->GetResetPosition() );		
		pcSprite->SetParent( rcCurrentLayer );
		pcSprite->SetName( pszName );

		return pcSprite;
	}	
}

// Finds and returns an object from the pool.
namespace FindObject
{
	inline CGCObjEnemy* Enemy()
	{
		CGCObjEnemy* pcEnemy;
		CGCObject* pcBaseObject;
		pcBaseObject = CGCObjectManager::FindObject( "KongExtend", GetGCTypeIDOf( CGCObjEnemy ) );
		return pcEnemy = static_cast<CGCObjEnemy*>(pcBaseObject);
	}

	inline CTriggerPlatform* Platform()
	{
		CTriggerPlatform* pcTriggerPlatform;
		CGCObject* pcBasePlatform;
		pcBasePlatform = CGCObjectManager::FindObject( "TriggerPlatform", GetGCTypeIDOf( CTriggerPlatform ) );
		return pcTriggerPlatform = static_cast<CTriggerPlatform*>(pcBasePlatform);
	}

	inline CDoor* Door()
	{
		CDoor* pcDoor;
		CGCObject* pcBaseObject;
		pcBaseObject = CGCObjectManager::FindObject( "Door", GetGCTypeIDOf( CDoor ) );
		return pcDoor = static_cast<CDoor*>(pcBaseObject);	
	}
}

#ifdef _DEBUG
#define ASSERT_CHECK(param) assert(param)
#endif


#endif // !HELPERS_H
