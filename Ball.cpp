#include <stdlib.h>
#include <time.h>

class Ball : public Object { 

protected:

public: 
	Ball(Ogre::SceneManager* scnMgr, btVector3 vector) : Object(scnMgr){

		startPos = vector;
		
		ent = scnMgr->createEntity("sphere.mesh");
		ent->setMaterialName("ball/Green");

		node = scnMgr->getRootSceneNode()->createChildSceneNode();
		node->attachObject(ent);
		node->setScale(Ogre::Vector3(0.04,0.04,0.04));

		colShape = new btSphereShape(btScalar(4));

		btTransform startTransform;
        startTransform.setIdentity();

        mass = btScalar(.1f);

        btVector3 localInertia(0,0,0);
        
        if (isDynamic())
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(vector);

        motionState = new MyMotionState(startTransform, node);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
        body = new btRigidBody(rbInfo);

        body->setRestitution(1.0);
        body->setFriction(0.0);
        body->setDamping(0, 0);
        srand(time(NULL)); //initialized based on time to make it "random"
        randomizeColor();
	}

	~Ball(){}; 

	void push(){
		body->activate(true);
		body->applyCentralImpulse( btVector3(1, 1, 2));
	}

	void randomizeColor(){
		color = (rand() % 3);
		//0 Blue, 1 Red, 2 Green for now
		if(color == 0)
			ent->setMaterialName("paddle/Blue");
		if(color == 1)
			ent->setMaterialName("paddle/Red");
		if(color == 2)
			ent->setMaterialName("paddle/Green");
	}

};
