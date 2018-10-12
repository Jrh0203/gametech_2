class Wall : public Object { 

protected:

public: 
	Wall(Ogre::SceneManager* scnMgr, btVector3 vector, int yaw, int pitch, int roll) : Object(scnMgr){

    	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -50);

    	Ogre::MeshManager::getSingleton().createPlane("wall",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            plane,
            100,100,20,20,true,
            1,5,5,Ogre::Vector3::UNIT_Z);

    	ent = scnMgr->createEntity("wall");
    	ent->setMaterialName("Examples/Rockwall");
    	ent->setCastShadows(true);

    	node = scnMgr->getRootSceneNode()->createChildSceneNode();
    	node->attachObject(ent);

    	node->yaw(Ogre::Degree(yaw));
    	node->pitch(Ogre::Degree(pitch));
    	node->roll(Ogre::Degree(roll));


      if (pitch==90){
        //ent->setMaterialName("wall/Clear");
        ent->setVisible(false);
      }

   		int width = 100;
   		int height = 1;
   		int depth = 100;

   		//switch width and height
   		if (roll == 90 || roll == -90){

   			int temp = height;
   			height = width;
   			width=temp;

   		}

   		if ( pitch == 90 || pitch == -90){
   			int temp = depth;
   			depth = height;
   			height=temp;

   		}

        colShape = new btBoxShape(btVector3(btScalar(width),btScalar(height),btScalar(depth)));

        btTransform startTransform;
        startTransform.setIdentity();
        startTransform.setOrigin(vector);
        //startTransform.setOrigin(btVector3(0,-6,0));

        mass = btScalar(0.f);

        btVector3 localInertia(0,0,0);

        motionState = new MyMotionState(startTransform, node);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
        body = new btRigidBody(rbInfo);

        body->setRestitution(1.0);
        body->setFriction(0.0);
	}

	~Wall(){}; 
};