
#include "MyMotionState.cpp"

class Object { 
protected: 
	Ogre::SceneNode* node; 
	Ogre::Entity *ent;
	btRigidBody* body;
	btCollisionShape* colShape;
	btScalar mass;
	MyMotionState* motionState;
	btVector3 startPos;

public: 
	Object(Ogre::SceneManager* scnMgr){}

	~Object(){}

	Ogre::SceneNode* getNode() { return node; } 

	btRigidBody* getRigidBody(){return body;}

	btCollisionShape* getCollisionShape() { return colShape; }

	bool isDynamic() { return (mass != 0.f); }

	MyMotionState* getMotionState(){return motionState;}

	void reset(){
		btVector3 zeroVector(0, 0, 0);

		body->clearForces();
		body->setLinearVelocity(zeroVector);
		body->setAngularVelocity(zeroVector);

		
		btTransform transform;
		transform.setIdentity();

		transform.setOrigin(startPos);

		body->setWorldTransform(transform);
		motionState->setWorldTransform(transform);
	}

};
