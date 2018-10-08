class Ball : public Object { 

protected:
	int color;

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
        color = 0;
	}

	~Ball(){}; 

	void push(){
		body->activate(true);
		body->applyCentralImpulse( btVector3(1, 0, 3));
	}

	void explode(){
		reset();
	}



};
