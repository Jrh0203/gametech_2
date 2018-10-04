/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/


Copyright (c) 2000-2013 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

//! [starter]

#include <exception>

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreApplicationContext.h>
#include <OgreCameraMan.h>
#include "Ball.h"
#include "Physics.cpp"

using namespace Ogre;
using namespace OgreBites;

int i = 0;
Camera* cam;

class MyMotionState : public btMotionState {
public:
    MyMotionState(const btTransform &initialpos, Ogre::SceneNode *node) {
        mVisibleobj = node;
        mPos1 = initialpos;
    }
    virtual ~MyMotionState() {    }
    void setNode(Ogre::SceneNode *node) {
        mVisibleobj = node;
    }
    virtual void getWorldTransform(btTransform &worldTrans) const {
        worldTrans = mPos1;
    }
    virtual void setWorldTransform(const btTransform &worldTrans) {
        if(NULL == mVisibleobj) return; // silently return before we set a node
        btQuaternion rot = worldTrans.getRotation();
        mVisibleobj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
        btVector3 pos = worldTrans.getOrigin();
        // TODO **** XXX need to fix this up such that it renders properly since this doesnt know the scale of the node
        // also the getCube function returns a cube that isnt centered on Z
        mVisibleobj->setPosition(pos.x(), pos.y()+5, pos.z()-5);
    }
protected:
    Ogre::SceneNode *mVisibleobj;
    btTransform mPos1;
};

class TutorialApplication
        : public ApplicationContext
        , public InputListener
{
public:
    TutorialApplication();
    virtual ~TutorialApplication();

    void setup();
    bool keyPressed(const KeyboardEvent& evt);
    bool keyReleased(const KeyboardEvent& evt);
    void frameRendered(const Ogre::FrameEvent & evt );
    //bool frameStarted(const FrameEvent &evt);
    void setupScene(SceneManager* mSceneMgr);
    void createBulletSim();
    ManualObject* createCubeMesh(Ogre::String name, Ogre::String matName);
    Ball* b;
    bool* keys = new bool[6];
    SceneNode* camNode;

    Physics* physicsEngine;


    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btCollisionShape* groundShape;
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    SceneNode *boxNode;
};

ManualObject* TutorialApplication::createCubeMesh(Ogre::String name, Ogre::String matName) {

   ManualObject* cube = new ManualObject(name);

   cube->begin(matName);

   cube->position(0.5,-0.5,1.0);cube->normal(0.408248,-0.816497,0.408248);cube->textureCoord(1,0);
   cube->position(-0.5,-0.5,0.0);cube->normal(-0.408248,-0.816497,-0.408248);cube->textureCoord(0,1);
   cube->position(0.5,-0.5,0.0);cube->normal(0.666667,-0.333333,-0.666667);cube->textureCoord(1,1);
   cube->position(-0.5,-0.5,1.0);cube->normal(-0.666667,-0.333333,0.666667);cube->textureCoord(0,0);
   cube->position(0.5,0.5,1.0);cube->normal(0.666667,0.333333,0.666667);cube->textureCoord(1,0);
   cube->position(-0.5,-0.5,1.0);cube->normal(-0.666667,-0.333333,0.666667);cube->textureCoord(0,1);
   cube->position(0.5,-0.5,1.0);cube->normal(0.408248,-0.816497,0.408248);cube->textureCoord(1,1);
   cube->position(-0.5,0.5,1.0);cube->normal(-0.408248,0.816497,0.408248);cube->textureCoord(0,0);
   cube->position(-0.5,0.5,0.0);cube->normal(-0.666667,0.333333,-0.666667);cube->textureCoord(0,1);
   cube->position(-0.5,-0.5,0.0);cube->normal(-0.408248,-0.816497,-0.408248);cube->textureCoord(1,1);
   cube->position(-0.5,-0.5,1.0);cube->normal(-0.666667,-0.333333,0.666667);cube->textureCoord(1,0);
   cube->position(0.5,-0.5,0.0);cube->normal(0.666667,-0.333333,-0.666667);cube->textureCoord(0,1);
   cube->position(0.5,0.5,0.0);cube->normal(0.408248,0.816497,-0.408248);cube->textureCoord(1,1);
   cube->position(0.5,-0.5,1.0);cube->normal(0.408248,-0.816497,0.408248);cube->textureCoord(0,0);
   cube->position(0.5,-0.5,0.0);cube->normal(0.666667,-0.333333,-0.666667);cube->textureCoord(1,0);
   cube->position(-0.5,-0.5,0.0);cube->normal(-0.408248,-0.816497,-0.408248);cube->textureCoord(0,0);
   cube->position(-0.5,0.5,1.0);cube->normal(-0.408248,0.816497,0.408248);cube->textureCoord(1,0);
   cube->position(0.5,0.5,0.0);cube->normal(0.408248,0.816497,-0.408248);cube->textureCoord(0,1);
   cube->position(-0.5,0.5,0.0);cube->normal(-0.666667,0.333333,-0.666667);cube->textureCoord(1,1);
   cube->position(0.5,0.5,1.0);cube->normal(0.666667,0.333333,0.666667);cube->textureCoord(0,0);

   cube->triangle(0,1,2);      cube->triangle(3,1,0);
   cube->triangle(4,5,6);      cube->triangle(4,7,5);
   cube->triangle(8,9,10);      cube->triangle(10,7,8);
   cube->triangle(4,11,12);   cube->triangle(4,13,11);
   cube->triangle(14,8,12);   cube->triangle(14,15,8);
   cube->triangle(16,17,18);   cube->triangle(16,19,17);
   cube->end();

   return cube;
}


void TutorialApplication::createBulletSim(void) {
        ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
        collisionConfiguration = new btDefaultCollisionConfiguration();

        ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
        dispatcher = new    btCollisionDispatcher(collisionConfiguration);

        ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
        overlappingPairCache = new btDbvtBroadphase();

        ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
        solver = new btSequentialImpulseConstraintSolver;

        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0,-10,0));

        ///create a few basic rigid bodies
        // start with ground plane, 1500, 1500
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1500.),btScalar(1.),btScalar(1500.)));

        collisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0,-2,0));

        {
            btScalar mass(0.);

            //rigidbody is dynamic if and only if mass is non zero, otherwise static
            bool isDynamic = (mass != 0.f);

            btVector3 localInertia(0,0,0);
            if (isDynamic)
                groundShape->calculateLocalInertia(mass,localInertia);

            // lathe - this plane isnt going to be moving so i dont care about setting the motion state
            //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
            btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
            btRigidBody* body = new btRigidBody(rbInfo);

            //add the body to the dynamics world
            dynamicsWorld->addRigidBody(body);
        }


        {
            //create a dynamic rigidbody

            btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
  //            btCollisionShape* colShape = new btSphereShape(btScalar(1.));
            collisionShapes.push_back(colShape);

            /// Create Dynamic Objects
            btTransform startTransform;
            startTransform.setIdentity();

            btScalar    mass(1.f);

            //rigidbody is dynamic if and only if mass is non zero, otherwise static
            bool isDynamic = (mass != 0.f);

            btVector3 localInertia(0,0,-1.0);
            if (isDynamic)
                colShape->calculateLocalInertia(mass,localInertia);

                startTransform.setOrigin(btVector3(0,250,0));
                // *** give it a slight twist so it bouncees more interesting
                startTransform.setRotation(btQuaternion(btVector3(1.0, 1.0, 0.0), 0.6));

                //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
                //btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
                MyMotionState* motionState = new MyMotionState(startTransform, boxNode);
                btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,motionState,colShape,localInertia);
                btRigidBody* body = new btRigidBody(rbInfo);

                dynamicsWorld->addRigidBody(body);
        }
}

void TutorialApplication::setupScene(SceneManager* mSceneMgr)
    {
        //mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "Default SceneManager");
        //Camera *cam = mSceneMgr->createCamera("Camera");
        //Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);

        Entity *ent;

        //mSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));
        //mSceneMgr->setShadowTechnique( SHADOWTYPE_STENCIL_ADDITIVE );

        // make a cube to bounce around

        
        ManualObject *cmo = createCubeMesh("manual", "");
        cmo->convertToMesh("cube");
        ent = mSceneMgr->createEntity("Cube", "cube.mesh");
        ent->setCastShadows(true);
        boxNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        boxNode->attachObject(ent);
        boxNode->setScale(Vector3(0.05,0.05,0.05)); // for some reason converttomesh multiplied dimensions by 10
        

        // make a rock wall on the floor
        
        Plane plane(Vector3::UNIT_Y, 0);
        MeshManager::getSingleton().createPlane("ground",
                    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
                    150,150,20,20,true,1,5,5,Vector3::UNIT_Z);
        ent = mSceneMgr->createEntity("GroundEntity", "ground");
        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
        ent->setMaterialName("Examples/Rockwall");

        ent->setCastShadows(false);
        
        


        // make a light to see stuff with
        /*
        Light *light = mSceneMgr->createLight("Light1");
        light->setType(Light::LT_POINT);
        light->setPosition(Vector3(250, 150, 250));
        light->setDiffuseColour(ColourValue::White);
        light->setSpecularColour(ColourValue::White);
        */

        // Create the scene node
        //SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Vector3(-200, 100, 200));
        //node->yaw(Degree(-45));
        //node->attachObject(cam);
    }


TutorialApplication::TutorialApplication()
    : ApplicationContext("OgreTutorialApp")
{
}


TutorialApplication::~TutorialApplication()
{
}


void TutorialApplication::setup()
{
    // do not forget to call the base first
    for (int i = 0; i < 6; i++){
        keys[i]=false;
    }
    ApplicationContext::setup();
    addInputListener(this);

    // get a pointer to the already created root
    Root* root = getRoot();
    SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // -- tutorial section start --
    //! [cameracreate]
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    cam = scnMgr->createCamera("myCam");
    //! [cameracreate]

    //! [cameraposition]
    camNode->setPosition(-20, 10, 20);
    camNode->yaw(Degree(-45));
    //camNode->lookAt(Vector3(0, 0, -300), Node::TransformSpace::TS_WORLD);
    //! [cameraposition]

    //! [cameralaststep]
    cam->setNearClipDistance(5);
    camNode->attachObject(cam);
    //! [cameralaststep]

    //! [addviewport]
    Viewport* vp = getRenderWindow()->addViewport(cam);
    //! [addviewport]

    //! [viewportback]
    vp->setBackgroundColour(ColourValue(0, 0, 0));
    //! [viewportback]

    //! [cameraratio]
    cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    //! [cameraratio]

    Light* light = scnMgr->createLight("MainLight");
    SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setPosition(0, 10, 0);

    //scnMgr->setSkyBox(true, "Examples/MorningSkyBox", 5000, false);
    //! [ninja]
    /*
    Entity* sphereEntity = scnMgr->createEntity("sphere.mesh");

    sphereEntity->setCastShadows(true);
    
    SceneNode* sphere_scene = scnMgr->getRootSceneNode()->createChildSceneNode();
    sphere_scene->attachObject(sphereEntity);
    sphere_scene->setScale(.01, .01, .01);
    sphere_scene->setPosition(0,0,0);


    */
    //! [lightingsset]
    scnMgr->setAmbientLight(ColourValue(1, 1, 1));
    scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
    //! [lightingsset]

    setupScene(scnMgr);
    

    //b = new Ball(scnMgr);

    Plane plane(Vector3::UNIT_Y, -5);
    //! [planedefine]
    MeshManager::getSingleton().createPlane(
            "plane",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            plane,
            10, 10, 20, 20,
            true,
            1, 5, 5,
            Vector3::UNIT_Z);

    Entity** walls_entity = new Entity*[6];
    SceneNode** walls_scene = new SceneNode*[6];

    for (int i = 0; i < 0; i++){
        std::string name = "plane"+std::to_string(i);
        std::cerr << name << '\n';
        walls_entity[i] = scnMgr->createEntity("plane");
        walls_scene[i] = scnMgr->getRootSceneNode()->createChildSceneNode();
        walls_scene[i]->attachObject(walls_entity[i]);
        walls_entity[i]->setCastShadows(false);
        walls_entity[i]->setMaterialName("Examples/Rockwall");
        if (i==1){
            walls_scene[i]->roll(Degree(90));
        }
        if (i==2){
            walls_scene[i]->roll(Degree(-90));
        }
        if (i==3){
            walls_scene[i]->pitch(Degree(90));
        }
        if (i==4){
            walls_scene[i]->pitch(Degree(-90));
        }
        if (i==5){
            walls_scene[i]->roll(Degree(180));
        }
    }
    
    //! [planesetmat]

    

    
}


bool TutorialApplication::keyPressed(const KeyboardEvent& evt)
{
    std::cerr << "Key! " << '\n';
    if (evt.keysym.sym == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    if (evt.keysym.sym == 'r')
    {
        b->Ball::reset();
    }
    if (evt.keysym.sym == 'w')
    {
        keys[0]=true;
    }
    if (evt.keysym.sym == 'a')
    {
        keys[1]=true;
    }
    if (evt.keysym.sym == 's')
    {
        keys[2]=true;
    }
    if (evt.keysym.sym == 'd')
    {
        keys[3]=true;
    }
    if (evt.keysym.sym == 'e')
    {
        keys[4]=true;
    }
    if (evt.keysym.sym == 'q')
    {
        keys[5]=true;
    }
    return true;
}

bool TutorialApplication::keyReleased(const KeyboardEvent& evt)
{
    std::cerr << "Key! " << '\n';
    if (evt.keysym.sym == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    if (evt.keysym.sym == 'w')
    {
        keys[0]=false;
    }
    if (evt.keysym.sym == 'a')
    {
        keys[1]=false;
    }
    if (evt.keysym.sym == 's')
    {
        keys[2]=false;
    }
    if (evt.keysym.sym == 'd')
    {
        keys[3]=false;
    }
    if (evt.keysym.sym == 'e')
    {
        keys[4]=false;
    }
    if (evt.keysym.sym == 'q')
    {
        keys[5]=false;
    }
    return true;
}

void TutorialApplication::frameRendered(const Ogre::FrameEvent & evt ){
    
    //b->Ball::move(evt);

    if (keys[0]==true){
        camNode->translate(0,0,-.1);
    }
    if (keys[1]==true){
        camNode->translate(-.1,0,0);
    }
    if (keys[2]==true){
        camNode->translate(0,0,.1);
    }
    if (keys[3]==true){
        camNode->translate(.1,0,0);
    }
    if (keys[4]==true){
        camNode->translate(0,.1,0);
    }
    if (keys[5]==true){
        camNode->translate(0,-.1,0);
    }
    //dynamicsWorld->stepSimulation(evt.timeSinceLastFrame,50);
    //return true;
    //return mContinue;
    dynamicsWorld->stepSimulation(evt.timeSinceLastFrame,5);
}

int main(int argc, char **argv)
{
    try
    {
        TutorialApplication app;
        app.initApp();
        app.createBulletSim();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

//! [starter]

