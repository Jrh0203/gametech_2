#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include <vector>
#include <map>
#include <iostream>

class Physics{

	btDefaultCollisionConfiguration* collisionConfiguration
		= new btDefaultCollisionConfiguration();;
	btCollisionDispatcher* dispatcher
		= new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache
		=new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver
		= new btSequentialImpulseConstraintSolver();
     btDiscreteDynamicsWorld* dynamicsWorld 
     	= new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, 
     		solver, collisionConfiguration);
	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;
	
	public:
		btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; }
		std::vector<btCollisionShape *> &getCollisionShapes() { 
			std::cout << "gettingCollisionShapes" << std::endl;
			return collisionShapes;}
		int getCollisionObjectCount() { 
			int size =static_cast<int>(collisionShapes.size());
			std::cout << "Size: " + std::to_string(size) << std::endl;
			return size; }


};