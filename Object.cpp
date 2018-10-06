
#include "MyMotionState.cpp"

class Object { 
protected: 
	Ogre::SceneNode* node; 
	Ogre::Entity *ent;
	btRigidBody* body;
	btCollisionShape* colShape;
	btScalar mass;
	MyMotionState* motionState;

public: 
	Object(Ogre::SceneManager* scnMgr){}

	~Object(){}

	Ogre::SceneNode* getNode() { return node; } 

	btRigidBody* getRigidBody(){return body;}

	btCollisionShape* getCollisionShape() { return colShape; }

	bool isDynamic() { return (mass != 0.f); }

	MyMotionState* getMotionState(){return motionState;}

};
