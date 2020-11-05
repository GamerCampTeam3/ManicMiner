////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJSPRITEPHYSICS_H_
#define _GCOBJSPRITEPHYSICS_H_

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef _GCOBJSPRITE_H_
	#include "GCObjSprite.h"
#endif

#ifndef BOX2D_H
	#include "Box2D/Box2D.h"
#endif

#ifndef _SGCFACTORYCREATIONPARAMS_H_
	#include "SGCFactoryCreationParams.h"
#endif

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics is intended for use as a base class for any CGCObject 
// derived types that need to render a sprite. 
// 
// It wraps the CCSprite class, taking responsibility for managing the 
// lifetime of the managed CCSprite.
//
// The default behaviour is to make the sprite visible when it is put into 
// the live list of the containing group, and to make it invisible when it 
// is put into the dead list of the containing group.
//////////////////////////////////////////////////////////////////////////
class CGCObjSpritePhysics
: public CGCObjSprite
{
private:
	//////////////////////////////////////////////////////////////////////////
	// parameters passed to this object from the factory when it was created
	// this pointer is set during CGCObjSpritePhysics::VHandleFactoryParams()
	const CGCFactoryCreationParams* m_psCreateParams;	
	
	// box 2d body
	b2Body*			m_pb2Body;
	b2BodyDef		m_b2BodyDef;
	std::string		m_strShapeName;
	cocos2d::Vec2	m_v2LastMoveVelocity;

protected:
	CGCObjSpritePhysics( GCTypeID idDerivedType );

	inline b2Body* GetPhysicsBody();

public:
	inline const CGCFactoryCreationParams* GetFactoryCreationParams()	const;

							CGCObjSpritePhysics();
	virtual					~CGCObjSpritePhysics();

	void					InitBox2DParams( const b2BodyDef& rBodyDef, const char* pszShapeName );

	virtual void			VHandleFactoryParams( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition ); 

	// called once per game frame to update gfx posiitons from b2d
	virtual void			VUpdateSpriteFromBody( const b2Body* pcb2Body );

	inline cocos2d::Vec2	GetVelocity() const;
	inline void				SetVelocity( cocos2d::Vec2 v2NewVelocity );

	// applying a force to the center of an object guarantees no torques (rotational accelerations) are introduced
	inline void				ApplyForceToCenter( cocos2d::Vec2 v2Force );	

	inline b2Transform		GetPhysicsTransform() const;

	// don't use this to implement movement! this is for resetting / intialising etc.
	inline void				SetPhysicsTransform( const cocos2d::Vec2& rv2Pos, float fAngle );

	// in general you should use forces or velocity to move an object.
	// if you have a kinematic rigidbody (which is part of the physics simulation but 
	// doesn't have its forces / velocity / rotation / position updated by the engine)
	// then you can should this function to move the object.
	// this function sets the object's velocity to move it to the required position in 1 frame
	// note: this requires a little upkeep in object state: see m_v2LastMoveVelocity
	void					MoveToPixelPosition( const cocos2d::Vec2& rv2Pos );


	//////////////////////////////////////////////////////////////////////////
	// CGCObject Interface - see CGCObject for explanation of the purpose
	// and responsibilities of these functions
		virtual void VOnResourceAcquire () override; 
		virtual void VOnReset			() override;
		virtual void VOnKilled			() override;
		virtual void VOnResurrected		() override;
		virtual void VOnResourceRelease	() override;
	// CGCObject Interface
	//////////////////////////////////////////////////////////////////////////

	static inline const b2Fixture* 		FromB2DContactGetFixture_A		( const b2Contact* pcContact );
	static inline const b2Fixture*		FromB2DContactGetFixture_B		( const b2Contact* pcContact );
	static inline CGCObjSpritePhysics*	FromB2DFixtureGetSpritePhysics	( const b2Fixture* pFixture );
};


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// protected
inline b2Body* CGCObjSpritePhysics::GetPhysicsBody()
{
	return m_pb2Body;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// protected
inline const CGCFactoryCreationParams* CGCObjSpritePhysics::GetFactoryCreationParams() const
{
	return m_psCreateParams;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline cocos2d::Vec2 CGCObjSpritePhysics::GetVelocity() const
{
	b2Vec2 b2v2Vel = m_pb2Body->GetLinearVelocity();
	return cocos2d::Vec2( b2v2Vel.x, b2v2Vel.y );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void	CGCObjSpritePhysics::SetVelocity( cocos2d::Vec2 v2NewVelocity )
{
	m_pb2Body->SetLinearVelocity( b2Vec2( v2NewVelocity.x, v2NewVelocity.y ) );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void	CGCObjSpritePhysics::ApplyForceToCenter( cocos2d::Vec2 v2Force )
{
	m_pb2Body->ApplyForceToCenter( b2Vec2( v2Force.x, v2Force.y ), true );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline b2Transform CGCObjSpritePhysics::GetPhysicsTransform() const
{
	return m_pb2Body->GetTransform();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void	CGCObjSpritePhysics::SetPhysicsTransform( const cocos2d::Vec2& rv2Pos, float fAngle )
{
	m_pb2Body->SetTransform( b2Vec2( rv2Pos.x, rv2Pos.y ), fAngle );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
inline const b2Fixture* CGCObjSpritePhysics::FromB2DContactGetFixture_A( const b2Contact* pcContact )
{
	CCAssert( ( nullptr != pcContact ), "pcContact is nullptr" );
	return pcContact->GetFixtureA();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
inline const b2Fixture* CGCObjSpritePhysics::FromB2DContactGetFixture_B( const b2Contact* pcContact )
{
	CCAssert( ( nullptr != pcContact ), "pcContact is nullptr" );
	return pcContact->GetFixtureB();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
inline CGCObjSpritePhysics* CGCObjSpritePhysics::FromB2DFixtureGetSpritePhysics( const b2Fixture* pFixture )
{
	CCAssert( ( nullptr != pFixture ), "pFixture is nullptr" );

	const b2Body* pBody = pFixture->GetBody();

	if(		( nullptr != pBody )
		&&	( nullptr != pBody->GetUserData() ) )
	{
		return (CGCObjSpritePhysics*) pBody->GetUserData();
	}

	return nullptr;
}

#endif