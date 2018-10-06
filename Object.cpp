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
	btScalar mass;
	MyMotionState* motionState;

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

		colShape = new btSphereShape(btScalar(1));

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
         std::cout << "rb made" <<std::endl;
	}

	~Object(){}; 

	Ogre::SceneNode* getNode() { return node; } 

	btRigidBody* getRigidBody(){return body;}

	btCollisionShape* getCollisionShape() { return colShape; }

	bool isDynamic() { return (mass != 0.f); }

	MyMotionState* getMotionState(){return motionState;}

	void reset(){
		std::cout << "reset pressed" <<std::endl;
		btTransform trans;
		std::cout << "getting motion state" <<std::endl;
		motionState->getWorldTransform(trans);
		std::cout << "got motion state" <<std::endl;
		void *userPointer = body->getUserPointer();
		if (userPointer) {
			btQuaternion orientation = trans.getRotation();
			node->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
			node->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
		}
			
	}
};
