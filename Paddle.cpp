#include <Ogre.h> 
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "Object.cpp"
class Paddle : public Object{ 

public: 
	Paddle (Ogre::SceneManager* scnMgr, btVector3 vector, Ogre::Entity* ent) 
	: Object(scnMgr, vector, ent){
		
	node->setScale(Ogre::Vector3(0.05,0.05,0.05));

	//overwrite body from super class
	colShape = new btBoxShape(btVector3(2.5,2.5,2.5));

	btTransform startTransform;
    startTransform.setIdentity();

    mass = btScalar(1.f);

    btVector3 localInertia(0,0,-1.0);
        
    if (isDynamic())
        colShape->calculateLocalInertia(mass, localInertia);

    startTransform.setOrigin(vector);

    motionState = new MyMotionState(startTransform, node);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
    body = new btRigidBody(rbInfo);

	}
 
	~Paddle() = default;

	void swing(){
		//swing the paddle or something idk
	}
};