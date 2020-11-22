#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include "GCLevelLoader_Ogmo.h"

#include "cocos2d.h"

#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"

//#include "GCLevelLoader_Ogmo.h"

using namespace std;
USING_NS_CC;



//////////////////////////////////////////////////////////////////////////
// constants defining the names of the xml expected attributes in the oel
// level file
//////////////////////////////////////////////////////////////////////////
static const char* k_pszXmlAttr_ClassName				= "class";
static const char* k_pszXmlAttr_PlistFile				= "PlistFile";
static const char* k_pszXmlAttr_PhysicsShape			= "shape";
static const char* k_pszXmlAttr_B2D_BodyType			= "B2DBodyType";
static const char* k_pszXmlAttr_B2D_HasFixedRotation	= "HasFixedRotation";
static const char* k_pszXmlAttr_StartPos_X				= "x";
static const char* k_pszXmlAttr_StartPos_Y				= "y";



// strings for b2BodyType enum
static const char* k_pszB2BodyType_Dynamic		= "b2_dynamicBody";
static const char* k_pszB2BodyType_Static		= "b2_staticBody";
static const char* k_pszB2BodyType_Kinematic	= "b2_kinematicBody";

const tinyxml2::XMLElement* CGCLevelLoader_Ogmo::sm_pCurrentObjectXmlData = nullptr;


b2Vec2 CGCLevelLoader_Ogmo::m_v2LevelDimensions = b2Vec2_zero;

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CGCLevelLoader_Ogmo::CGCLevelLoader_Ogmo( void )
//: m_v2LevelDimensions( b2Vec2_zero )
{
	m_vecFactoryCreationParams.reserve( EMAX_HANDLED_UNIQUE_CREATIONPARAMS );
	m_vecFactoryCreatedObjects.reserve( EMAX_HANDLED_OBJECTS );
}


//////////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////////
CGCLevelLoader_Ogmo::~CGCLevelLoader_Ogmo( void )
{	
	for( u32 u = 0; u < m_vecFactoryCreationParams.size(); ++u )
	{
		delete m_vecFactoryCreationParams[ u ];
	}
}


//////////////////////////////////////////////////////////////////////////
// This function just loads the level file.
// CreateObjects() must be called to create them
//////////////////////////////////////////////////////////////////////////
bool CGCLevelLoader_Ogmo::LoadLevelFile( const char* pszOelFile )
{
	//=== 1. XML-file
	Data data = FileUtils::getInstance()->getDataFromFile( pszOelFile );
	
	if( data.getSize() > 0 )
	{
		m_xmlOgmoDocument.Parse( (const char*) data.getBytes(), data.getSize() );

		if( m_xmlOgmoDocument.Error() )
		{
			cerr << " CGCLevelLoader_Ogmo: Error during parsing of xml-file!" << endl;

			// Print details about the error / Print the "parsing status"
			cerr << " xml-file [" << pszOelFile << "] was parsed with error: " << m_xmlOgmoDocument.ErrorID() << "\n";
			cerr << " Error description: " << m_xmlOgmoDocument.GetErrorStr1() << m_xmlOgmoDocument.GetErrorStr2() << "\n";

			CCAssert( false, " CGCLevelLoader_Ogmo: Failed to load xml-file!" );
		}
		else
		{
			m_pxmlLevelNode		= m_xmlOgmoDocument.RootElement();
			std::string strName	= m_pxmlLevelNode->Name();

			CCAssert( !strcmp( m_pxmlLevelNode->Name(), "level" ), "oel file appears to be incorrect format" );

			m_v2LevelDimensions = b2Vec2( m_pxmlLevelNode->FloatAttribute( "width" ), m_pxmlLevelNode->FloatAttribute( "height" ) );
		}

	}
	else
	{
		CCAssert( false, " CGCLevelLoader_Ogmo: Failed to open oel file" );
	}

	// successful
	return true;
}


//////////////////////////////////////////////////////////////////////////
//    tiny xml * tiny xml * tiny xml * tiny xml * tiny xml * tiny xml
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// tinyxml version of the file parser
class OgmoVisitor 
	: public tinyxml2::XMLVisitor
{
	CGCLevelLoader_Ogmo&			m_rOgmoLoader;
	CGCFactory_ObjSpritePhysics&	m_rFactoryClass;
	int								m_iNumCreated;

public:
	OgmoVisitor( CGCLevelLoader_Ogmo& rOgmoLoader, CGCFactory_ObjSpritePhysics& rClassFactory )
		: m_rOgmoLoader		( rOgmoLoader )
		, m_rFactoryClass	( rClassFactory )
		, m_iNumCreated		( 0 )
	{}

	virtual bool VisitEnter( const tinyxml2::XMLElement& rxmlElement, const tinyxml2::XMLAttribute* pxmlAttribute ) override
	{
		if( m_rOgmoLoader.AddFactoryDataForXMLElementAndCreateInstance( rxmlElement, m_rFactoryClass ) )
		{
			++m_iNumCreated;
		}
		return true;
	}

	int NumCreated()
	{
		return m_iNumCreated;
	}
};


//////////////////////////////////////////////////////////////////////////
inline bool CGCLevelLoader_Ogmo::AddFactoryDataForXMLElementAndCreateInstance( const tinyxml2::XMLElement& rxmlElement, CGCFactory_ObjSpritePhysics& rClassFactory )
{
	if( IsValidFactoryData( rxmlElement ) )
	{
		const CGCFactoryCreationParams* pParams = GetCreationParamsFromCache( GetFactoryClassName( rxmlElement ) );

		if( nullptr == pParams )
		{
			pParams = AddCreationParamsToInternalStore( GetFactoryData( rxmlElement ) );
		}

		cocos2d::Vec2 v2Pos = GetObjectPosition( rxmlElement );
		CCLOG( "creating instance of %s at position [%f, %f]", pParams->strClassName.c_str(), v2Pos.x, v2Pos.y );

		CCAssert( ( m_vecFactoryCreatedObjects.size() < EMAX_HANDLED_OBJECTS ),
				  "You need to reserve more space in CGCLevelLoader_Ogmo::m_vecFactoryCreatedObjects! Make CGCLevelLoader_Ogmo::EMAX_HANDLED_OBJECTS bigger!" );



		// filthy hack
		sm_pCurrentObjectXmlData = &rxmlElement;
		// filthy hack





		m_vecFactoryCreatedObjects.push_back( rClassFactory.CreateInstance( ( *pParams ), v2Pos ) );





		// filthy hack
		sm_pCurrentObjectXmlData = nullptr;
		// filthy hack






		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// validates an xml node as data that can be used to create an instance
// via the class factory
//////////////////////////////////////////////////////////////////////////
// private
inline bool CGCLevelLoader_Ogmo::IsValidFactoryData( const tinyxml2::XMLElement& rxmlElement )
{
	const char*	pszName						= rxmlElement.Name();
	const tinyxml2::XMLAttribute* pName		= rxmlElement.FindAttribute( k_pszXmlAttr_ClassName );
	const tinyxml2::XMLAttribute* pPlist	= rxmlElement.FindAttribute( k_pszXmlAttr_PlistFile );
	const tinyxml2::XMLAttribute* pShape	= rxmlElement.FindAttribute( k_pszXmlAttr_PhysicsShape );
	const tinyxml2::XMLAttribute* pBodyType	= rxmlElement.FindAttribute( k_pszXmlAttr_B2D_BodyType );
	const tinyxml2::XMLAttribute* pFixedRot	= rxmlElement.FindAttribute( k_pszXmlAttr_B2D_HasFixedRotation );
	const tinyxml2::XMLAttribute* pPosX		= rxmlElement.FindAttribute( k_pszXmlAttr_StartPos_X );
	const tinyxml2::XMLAttribute* pPosY		= rxmlElement.FindAttribute( k_pszXmlAttr_StartPos_Y );

	return(		( nullptr != pName		)
			&&	( nullptr != pPlist		)
			&&	( nullptr != pShape		)
			&&	( nullptr != pBodyType	)
			&&	( nullptr != pFixedRot	)
			&&	( nullptr != pPosX		)
			&&	( nullptr != pPosY		)
			);
}

//////////////////////////////////////////////////////////////////////////
// private  
inline const char* CGCLevelLoader_Ogmo::GetFactoryClassName( const tinyxml2::XMLElement& rxmlElement )
{
	return rxmlElement.Attribute( k_pszXmlAttr_ClassName );
}

//////////////////////////////////////////////////////////////////////////
// creates the corresponding factory creation params for a valid xml node
//////////////////////////////////////////////////////////////////////////
// private
inline const CGCFactoryCreationParams* CGCLevelLoader_Ogmo::GetFactoryData( const tinyxml2::XMLElement& rxmlElement )
{
	return( new CGCFactoryCreationParams(	rxmlElement.Attribute( k_pszXmlAttr_ClassName ),
			rxmlElement.Attribute( k_pszXmlAttr_PlistFile ),
			rxmlElement.Attribute( k_pszXmlAttr_PhysicsShape ),
			B2BodyTypeFromString( rxmlElement.Attribute( k_pszXmlAttr_B2D_BodyType ) ),
			rxmlElement.BoolAttribute( k_pszXmlAttr_B2D_HasFixedRotation ) 
			) );
}

//////////////////////////////////////////////////////////////////////////
// private  
inline cocos2d::Vec2 CGCLevelLoader_Ogmo::GetObjectPosition( const tinyxml2::XMLElement& rxmlElement )
{
	// N.B. we use the screen origin as the level position origin
	// this means that the bottom left of your level will always be visible, but the top right not guaranteed
	// resolution and sprite scale settings are all in AppDelegate.cpp - good luck!
	Point			origin = Director::getInstance()->getVisibleOrigin();
	cocos2d::Vec2	v2Origin( origin.x, origin.y );


	return (v2Origin + cocos2d::Vec2(rxmlElement.FloatAttribute(k_pszXmlAttr_StartPos_X),
		(m_v2LevelDimensions.y - rxmlElement.FloatAttribute(k_pszXmlAttr_StartPos_Y))));


}


//////////////////////////////////////////////////////////////////////////
//    tiny xml * tiny xml * tiny xml * tiny xml * tiny xml * tiny xml
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// recursively iterates he xml nodes in the loaded level document
// for each one it checks if it is a valid chunk of class factory data
// and if so uses it to create an object via the class factory
//////////////////////////////////////////////////////////////////////////
u32	CGCLevelLoader_Ogmo::CreateObjects( CGCFactory_ObjSpritePhysics& rcClassFactory )
{
	OgmoVisitor cOgmoVisitor( *this, rcClassFactory );

	m_xmlOgmoDocument.Accept( &cOgmoVisitor );

	return cOgmoVisitor.NumCreated();
}

//////////////////////////////////////////////////////////////////////////
// delete all the ObjSpritePhysics derived objects we created
//////////////////////////////////////////////////////////////////////////
void CGCLevelLoader_Ogmo::DestroyObjects()
{
	for( CGCObjSpritePhysics* pCreatedObj : m_vecFactoryCreatedObjects )
	{
		delete pCreatedObj;
	}

	m_vecFactoryCreatedObjects.clear();
}


//////////////////////////////////////////////////////////////////////////
// converts the xml string version of the box 2d body type to the enum
// value expected by box 2d
//////////////////////////////////////////////////////////////////////////
// private
inline b2BodyType CGCLevelLoader_Ogmo::B2BodyTypeFromString( const char* pszBodyType )
{
	b2BodyType eReturnType = b2_dynamicBody;

	// N.B. can't use a switch because of strings 
	if( !strcmp( pszBodyType, k_pszB2BodyType_Static ) )
	{
		eReturnType = b2_staticBody;

	}
	else if( !strcmp( pszBodyType, k_pszB2BodyType_Kinematic ) )
	{
		eReturnType = b2_kinematicBody;
	}

	return eReturnType;
}


//////////////////////////////////////////////////////////////////////////
// private
const CGCFactoryCreationParams* CGCLevelLoader_Ogmo::GetCreationParamsFromCache( const char* pszClassName )
{
	const CGCFactoryCreationParams* pcReturn = nullptr;

	size_t uNumElements = m_vecFactoryCreationParams.size();
	for( size_t u = 0; u < uNumElements; ++u )
	{
		if( m_vecFactoryCreationParams[ u ]->NameIs( pszClassName ) )
		{
			pcReturn = m_vecFactoryCreationParams[ u ]; 
			break;
		}
	}

	return pcReturn;
}


//////////////////////////////////////////////////////////////////////////
// this class used to store cache creation params by value, but has been 
// changed so that they're stored as pointers so that the data can be 
// extended
//////////////////////////////////////////////////////////////////////////
// private
const CGCFactoryCreationParams* CGCLevelLoader_Ogmo::AddCreationParamsToInternalStore( const CGCFactoryCreationParams* ptParams )
{
	CCAssert(	( m_vecFactoryCreationParams.size() < EMAX_HANDLED_UNIQUE_CREATIONPARAMS ), 
				"You need to reserve more space in CGCLevelLoader_Ogmo::m_vecFactoryCreationParams! Make CGCLevelLoader_Ogmo::EMAX_HANDLED_UNIQUE_CREATIONPARAMS bigger!" );

	m_vecFactoryCreationParams.push_back( ptParams );
	return m_vecFactoryCreationParams.back();
}
