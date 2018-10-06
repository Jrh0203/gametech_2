class Wall : public Object { 

protected:

public: 
	Wall(Ogre::SceneManager* scnMgr, btVector3 vector, 
		Ogre::Degree pitch, Ogre::Degree roll) : Object(scnMgr){
		
    	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

    	Ogre::MeshManager::getSingleton().createPlane("ground",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            plane,
            150,100,20,20,true,
            1,5,5,Ogre::Vector3::UNIT_Z);

    	ent = scnMgr->createEntity("ground");
    	ent->setMaterialName("Examples/Rockwall");
    	ent->setCastShadows(false);

    	node = scnMgr->getRootSceneNode()->createChildSceneNode();
    	node->attachObject(ent);

    	node->pitch(pitch);
    	node->roll(roll);

        colShape = new btBoxShape(btVector3(btScalar(1500.),btScalar(1.),btScalar(1500.)));

        btTransform startTransform;
        startTransform.setIdentity();
        //startTransform.setOrigin(vector);
        startTransform.setOrigin(btVector3(0,-6,0));

        mass = btScalar(0.f);

        btVector3 localInertia(0,0,0);

        motionState = new MyMotionState(startTransform, node);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
        body = new btRigidBody(rbInfo);

        body->setRestitution(0.0);
        body->setFriction(0.0);
	}

	~Wall(){}; 
};