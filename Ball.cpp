class Ball : public Object { 

protected:

public: 
	Ball(Ogre::SceneManager* scnMgr, btVector3 vector) : Object(scnMgr){
		
		ent = scnMgr->createEntity("sphere.mesh");

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
	}

	~Ball(){}; 

	void reset(){
		btVector3 zeroVector(0,0,0);

		body->clearForces();
		body->setLinearVelocity(zeroVector);
		body->setAngularVelocity(zeroVector);

		
		btTransform transform;
		transform.setIdentity();

		transform.setOrigin(zeroVector);

		body->setWorldTransform(transform);
		motionState->setWorldTransform(transform);
	}

	void push(){
		body->activate(true);
		body->applyCentralImpulse( btVector3(0, 0, -50));
	}
};
