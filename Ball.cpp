#include <OgreEntity.h> 
#include <OgreSceneManager.h> 
#include "Ball.h"
Ball::Ball(Ogre::SceneManager* scnMgr) { 
	srand (time(NULL));
	Ogre::Entity* ball = scnMgr->createEntity("Sphere", "sphere.mesh"); 
	ball->setMaterialName("Examples/Water7");
	ball->setCastShadows(true); 
	rootNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	rootNode->attachObject(ball); 
	rootNode->scale(0.01f,0.01f,0.01f); 
	rootNode->setPosition(0,0,0);
	bRadius = 1.0f; 
	bDirection = Ogre::Vector3(rand()*2-1, rand()*2-1, rand()*2-1); 
	bDirection.normalise(); 
	bSpeed = Ogre::Math::RangeRandom(1,5)+5; 

} 
void Ball::move(const Ogre::FrameEvent& evt) { 
	float room_dim = 10;

	Ogre::Vector3 bPosition = rootNode->getPosition(); 
	if (bPosition.y < -room_dim/2.0f + bRadius && bDirection.y < 0.0f) bDirection.y = -bDirection.y; 
	if (bPosition.y > room_dim/2.0f - bRadius && bDirection.y > 0.0f) bDirection.y = -bDirection.y; 
	if (bPosition.z < -room_dim/2.0f + bRadius && bDirection.z < 0.0f) bDirection.z = -bDirection.z; 
	if (bPosition.z > room_dim/2.0f - bRadius && bDirection.z > 0.0f) bDirection.z = -bDirection.z; 
	if (bPosition.x < -room_dim/2.0f + bRadius && bDirection.x < 0.0f) bDirection.x = -bDirection.x; 
	if (bPosition.x > room_dim/2.0f - bRadius && bDirection.x > 0.0f) bDirection.x = -bDirection.x; 
	rootNode->translate(bSpeed * evt.timeSinceLastFrame * bDirection); 
	
}

void Ball::reset() { 
	bDirection = Ogre::Vector3(rand()*2-1, rand()*2-1, rand()*2-1); 
	bDirection.normalise(); 
	bSpeed = Ogre::Math::RangeRandom(1,5)+5; 
	rootNode->setPosition(0,0,0);
}