#ifndef _GCLEVELLOADER_OGMO_H_
#define _GCLEVELLOADER_OGMO_H_

#include <vector>
#include <map>
#include <functional>


#ifndef _SGCFACTORYCREATIONPARAMS_H_
	#include "GamerCamp\GCCocosInterface\SGCFactoryCreationParams.h"
#endif

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif


#ifndef TINYXML2_INCLUDED
	#include "external\tinyxml2\tinyxml2.h"
#endif




//////////////////////////////////////////////////////////////////////////
// forward declaration
//////////////////////////////////////////////////////////////////////////
class CGCObjSpritePhysics;
class CGCFactory_ObjSpritePhysics;


//////////////////////////////////////////////////////////////////////////
// N.B. this code was written by one of the GCPro 2012 students
// and adapted and back ported by Alex
// TBH the orginial code was a bit of a state and I didn't have time to 
// clean it up as much as I would have liked...
// 
// N.B. you will see pugi::xml_node passed by value all over the place
// it is a VERY lightweight class - just one pointer internally
// 
//////////////////////////////////////////////////////////////////////////
class CGCLevelLoader_Ogmo
{
	tinyxml2::XMLDocument	m_xmlOgmoDocument;
	tinyxml2::XMLElement*	m_pxmlLevelNode;


	// level file has dimensions, need to use these to correctly position objects from level file
	// as ogmo's coords are top left 0,0 & cocos are bottom left 0,0
	b2Vec2				m_v2LevelDimensions;

	// vector of all unique parameter setups read from the level file
	// and used to create objects
	std::vector< const CGCFactoryCreationParams* >	m_vecFactoryCreationParams;
	enum
	{
	   EMAX_HANDLED_UNIQUE_CREATIONPARAMS = 256
	}; 

	// 
	std::vector< CGCObjSpritePhysics* >	m_vecFactoryCreatedObjects;
	enum
	{
		EMAX_HANDLED_OBJECTS = 1024
	};



	// adds the creation params to internal store
	const	CGCFactoryCreationParams*		AddCreationParamsToInternalStore		( const CGCFactoryCreationParams* rtParamsToCheck );
	const	CGCFactoryCreationParams*		GetCreationParamsFromCache				( const char* pszClassName );

	inline b2BodyType						B2BodyTypeFromString					( const char* pszBodyType );
	inline bool								IsValidFactoryData						( const tinyxml2::XMLElement& rxmlElement );
	inline const char*						GetFactoryClassName						( const tinyxml2::XMLElement& rxmlElement );
	inline const CGCFactoryCreationParams*	GetFactoryData							( const tinyxml2::XMLElement& rxmlElement );
	inline cocos2d::Vec2					GetObjectPosition						( const tinyxml2::XMLElement& rxmlElement );

public:
	inline bool								AddFactoryDataForXMLElementAndCreateInstance		( const tinyxml2::XMLElement& rxmlElement, CGCFactory_ObjSpritePhysics& rClassFactory );

public:
	// Con/destruction
	CGCLevelLoader_Ogmo( void );
	~CGCLevelLoader_Ogmo( void );

	// load level file (level_x.oel)
	bool			LoadLevelFile( const char* pszOelFile );

	// create objects
	u32				CreateObjects( CGCFactory_ObjSpritePhysics& rcClassFactory );

	// destroy objects
	void			DestroyObjects();
};


#endif