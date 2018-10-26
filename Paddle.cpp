
#include "Object.cpp"

class Paddle : public Object { 

protected:

public: 
	Paddle (Ogre::SceneManager* scnMgr, btVector3 vector, bool isPlayer) 
	: Object(scnMgr) {

		startPos = vector;

		 Ogre::ManualObject *cmo = createCubeMesh("manual", "");
	    cmo->convertToMesh("cube");

	    ent = scnMgr->createEntity("cube.mesh");

	    if(isPlayer){
			ent->setMaterialName("paddle/Blue");
		}else
			ent->setMaterialName("paddle/Red");
			
		node = scnMgr->getRootSceneNode()->createChildSceneNode();
		node->attachObject(ent);	
		node->setScale(Ogre::Vector3(0.05,0.05,0.05));

		//overwrite body from super class
		colShape = new btBoxShape(btVector3(2.5,2.5,2.5));

		btTransform startTransform;
	    startTransform.setIdentity();

	    mass = btScalar(50.f);

	    btVector3 localInertia(0,0,0);
	        
	    if (isDynamic())
	        colShape->calculateLocalInertia(mass, localInertia);

	    startTransform.setOrigin(vector);

	    motionState = new MyMotionState(startTransform, node);
	    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape, localInertia);
	    body = new btRigidBody(rbInfo);
	    body->setRestitution(1.0);
	    body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
		srand(time(NULL));
	    color = 0;
	    speed = 20;
	}
 
	~Paddle(){}

	void changeColor(int newColor = 0){
		color = newColor;

		//0 Blue, 1 Red, 2 Green for now
		if(color == 0)
			ent->setMaterialName("paddle/Blue");
		if(color == 1)
			ent->setMaterialName("paddle/Red");
		if(color == 2)
			ent->setMaterialName("paddle/Green");
	}

	void opponentChangeColor(int ballColor){
		int val = rand() % 100;

		int newColor;
		//opponent pcks correct color 90% of the time
		if (val < 90){
			newColor = ballColor;
		} else {
			newColor=(ballColor+1)%3;
		}

		changeColor(newColor);
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

	void playerUpdatePosition(double xUpdate, double yUpdate, double minX, double maxX, double minY, double maxY)
	{
		body->activate(true);
		btVector3 lvelocity(xUpdate*0.1, yUpdate*0.1, 0);
		body->translate(lvelocity);

		Ogre::Vector3 curPos = node->getPosition();
		btTransform transform;
		transform = body->getWorldTransform();
		if(transform.getOrigin().getX() < minX){
			transform.setOrigin(btVector3(minX, transform.getOrigin().getY(), transform.getOrigin().getZ()));
		}else if(transform.getOrigin().getX() > maxX){
			transform.setOrigin(btVector3(maxX, transform.getOrigin().getY(), transform.getOrigin().getZ()));
		}
		if(transform.getOrigin().getY() < minY){
			transform.setOrigin(btVector3(transform.getOrigin().getX(), minY, transform.getOrigin().getZ()));

		}else if(transform.getOrigin().getY() > maxY){
			transform.setOrigin(btVector3(transform.getOrigin().getX(), maxY, transform.getOrigin().getZ()));

		}

		body->setWorldTransform(transform);
		body->getMotionState()->setWorldTransform(transform);	
	}

	void movePaddleLocation(Ogre::Vector3 paddlePos){
		btTransform transform;
		transform = body->getWorldTransform();
		transform.setOrigin(btVector3(paddlePos.x, paddlePos.y, paddlePos.z));
		body->setWorldTransform(transform);
		body->getMotionState()->setWorldTransform(transform);
	}

	void updatePosition(Ogre::Vector3 ballPos){

		body->activate(true);	
		Ogre::Vector3 myPos = node->getPosition();
		int x=0, y=0;
		double minDistance = 3;
		if ((ballPos.x - myPos.x) < -minDistance){
			x-=speed;
		} else if ((ballPos.x - myPos.x) > minDistance){
			x+=speed;
		}
		if ((ballPos.y - myPos.y) < -minDistance){
			y-=speed;
		} else if ((ballPos.y - myPos.y) > minDistance){
			y+=speed;
		}
		body->setLinearVelocity(btVector3(x, y, 0));
	}

	void speedUp(float mult){
		speed*=mult;
	}

	Ogre::ManualObject* createCubeMesh(Ogre::String name, Ogre::String matName) {

	   Ogre::ManualObject* paddle = new Ogre::ManualObject(name);

	   paddle->begin(matName);

	   paddle->position(0.5,-0.5,1.0);paddle->normal(0.408248,-0.816497,0.408248);paddle->textureCoord(1,0);
	   paddle->position(-0.5,-0.5,0.0);paddle->normal(-0.408248,-0.816497,-0.408248);paddle->textureCoord(0,1);
	   paddle->position(0.5,-0.5,0.0);paddle->normal(0.666667,-0.333333,-0.666667);paddle->textureCoord(1,1);
	   paddle->position(-0.5,-0.5,1.0);paddle->normal(-0.666667,-0.333333,0.666667);paddle->textureCoord(0,0);
	   paddle->position(0.5,0.5,1.0);paddle->normal(0.666667,0.333333,0.666667);paddle->textureCoord(1,0);
	   paddle->position(-0.5,-0.5,1.0);paddle->normal(-0.666667,-0.333333,0.666667);paddle->textureCoord(0,1);
	   paddle->position(0.5,-0.5,1.0);paddle->normal(0.408248,-0.816497,0.408248);paddle->textureCoord(1,1);
	   paddle->position(-0.5,0.5,1.0);paddle->normal(-0.408248,0.816497,0.408248);paddle->textureCoord(0,0);
	   paddle->position(-0.5,0.5,0.0);paddle->normal(-0.666667,0.333333,-0.666667);paddle->textureCoord(0,1);
	   paddle->position(-0.5,-0.5,0.0);paddle->normal(-0.408248,-0.816497,-0.408248);paddle->textureCoord(1,1);
	   paddle->position(-0.5,-0.5,1.0);paddle->normal(-0.666667,-0.333333,0.666667);paddle->textureCoord(1,0);
	   paddle->position(0.5,-0.5,0.0);paddle->normal(0.666667,-0.333333,-0.666667);paddle->textureCoord(0,1);
	   paddle->position(0.5,0.5,0.0);paddle->normal(0.408248,0.816497,-0.408248);paddle->textureCoord(1,1);
	   paddle->position(0.5,-0.5,1.0);paddle->normal(0.408248,-0.816497,0.408248);paddle->textureCoord(0,0);
	   paddle->position(0.5,-0.5,0.0);paddle->normal(0.666667,-0.333333,-0.666667);paddle->textureCoord(1,0);
	   paddle->position(-0.5,-0.5,0.0);paddle->normal(-0.408248,-0.816497,-0.408248);paddle->textureCoord(0,0);
	   paddle->position(-0.5,0.5,1.0);paddle->normal(-0.408248,0.816497,0.408248);paddle->textureCoord(1,0);
	   paddle->position(0.5,0.5,0.0);paddle->normal(0.408248,0.816497,-0.408248);paddle->textureCoord(0,1);
	   paddle->position(-0.5,0.5,0.0);paddle->normal(-0.666667,0.333333,-0.666667);paddle->textureCoord(1,1);
	   paddle->position(0.5,0.5,1.0);paddle->normal(0.666667,0.333333,0.666667);paddle->textureCoord(0,0);

	   paddle->triangle(0,1,2);      paddle->triangle(3,1,0);
	   paddle->triangle(4,5,6);      paddle->triangle(4,7,5);
	   paddle->triangle(8,9,10);      paddle->triangle(10,7,8);
	   paddle->triangle(4,11,12);   paddle->triangle(4,13,11);
	   paddle->triangle(14,8,12);   paddle->triangle(14,15,8);
	   paddle->triangle(16,17,18);   paddle->triangle(16,19,17);
	   paddle->end();

	   return paddle;
}
};