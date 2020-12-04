//////////////////
/// Bib
//////////////////
///
#ifndef HELPERS_H
#define HELPERS_H

#include "GamerCamp/GCCocosInterface/GCObjSprite.h"
#include "ManicMiner/Layers/CManicLayer.h"

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
	
	inline CGCObjSprite* Create(  const char* pszPlist, float fZOrder, float fXScale, float fYScale, const cocos2d::Vec2 kv2SpritePos, CManicLayer* rcCurrentLayer)
	{
		CGCObjSprite* pcSprite = new CGCObjSprite();
		
		pcSprite->CreateSprite( pszPlist );
		pcSprite->SetSpriteGlobalZOrder( fZOrder );
		pcSprite->SetSpriteScale( fXScale, fYScale );
		pcSprite->SetResetPosition( kv2SpritePos );
		pcSprite->GetSprite()->setPosition( pcSprite->GetResetPosition() );		
		pcSprite->SetParent( rcCurrentLayer );

		return pcSprite;
	}


	
}

#ifdef _DEBUG
#define ASSERT_CHECK(param) assert(param)
#endif


#endif // !HELPERS_H
