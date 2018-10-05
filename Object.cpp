#include <Ogre.h> 
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "MyMotionState.cpp"
#include <iostream>

class Object { 
protected: 
	Ogre::SceneNode* node; 
	Ogre::Entity *ent;
	btRigidBody* body;
	btCollisionShape* colShape;

public: 
	Object(Ogre::SceneManager* scnMgr, btVector3 vector = btVector3(0, 0, 0),
	  Ogre::Entity* ent = NULL){
		      std::cout << "Creating object" <<std::endl;

		if (!ent){
			ent = scnMgr->createEntity("sphere.mesh");
		}

		node = scnMgr->getRootSceneNode()->createChildSceneNode();
		node->attachObject(ent);
		node->setScale(Ogre::Vector3(0.01,0.01,0.01));
		 std::cout << "node made" <<std::endl;

		colShape = new btSphereShape(btScalar(1));

		btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(1.f);
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,-1.0);
        
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(vector);

        MyMotionState* motionState = new MyMotionState(startTransform, node);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
        body = new btRigidBody(rbInfo);
         std::cout << "rb made" <<std::endl;
	}

	~Object(){}; 

	Ogre::SceneNode* getNode() { return node; } 

	btRigidBody* getRigidBody(){return body;}

	btCollisionShape* getCollisionShape() { return colShape; }
};
