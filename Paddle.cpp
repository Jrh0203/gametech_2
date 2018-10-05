#include <Ogre.h> 
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "Object.cpp"
class Paddle : public Object{ 

public: 
	Paddle (Ogre::SceneManager* scnMgr, btVector3 vector, Ogre::Entity* ent) 
	: Object(scnMgr, vector, ent){
		

	}
 
	~Paddle() = default;
};