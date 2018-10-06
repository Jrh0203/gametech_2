
#include "Object.cpp"

class Paddle : public Object { 

public: 
	Paddle (Ogre::SceneManager* scnMgr, btVector3 vector, Ogre::Entity* ent) 
	: Object(scnMgr) {
		
	node = scnMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(ent);	
	node->setScale(Ogre::Vector3(0.05,0.05,0.05));

	//overwrite body from super class
	colShape = new btBoxShape(btVector3(2.5,2.5,2.5));

	btTransform startTransform;
    startTransform.setIdentity();

    mass = btScalar(50.f);

    btVector3 localInertia(0,0,-1.0);
        
    if (isDynamic())
        colShape->calculateLocalInertia(mass, localInertia);

    startTransform.setOrigin(vector);

    motionState = new MyMotionState(startTransform, node);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
    body = new btRigidBody(rbInfo);
    body->setRestitution(1.0);
    body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));

	}
 
	~Paddle(){}

	void swing(){
		//swing the paddle or something idk
	}

	void moveLeft(){

		body->activate(true);
		btVector3 lvelocity(-15, 0, 0);
		body->setLinearVelocity(lvelocity);

	}

	void moveRight(){

		body->activate(true);
		btVector3 lvelocity(15, 0, 0);
		body->setLinearVelocity(lvelocity);

	}

	void stop(){
		body->activate(true);
		btVector3 lvelocity(0, 0, 0);
		body->setLinearVelocity(lvelocity);
	}
};